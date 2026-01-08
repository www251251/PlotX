#include "PlotRegistry.hpp"
#include "model/StorageModel.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/infra/IntEncoder.hpp"
#include "plotx/infra/Reflection.hpp"
#include "plotx/math/PlotCoord.hpp"

#include "ll/api/data/KeyValueDB.h"
#include "ll/api/io/Logger.h"
#include "mc/platform/UUID.h"

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "plotx/infra/NodeTree.hpp"

#include <algorithm>
#include <filesystem>
#include <ll/api/coro/CoroTask.h>
#include <ll/api/coro/InterruptableSleep.h>
#include <ll/api/thread/ThreadPoolExecutor.h>
#include <mutex>
#include <shared_mutex>
#include <string>

namespace plotx {

static constexpr std::string_view VersionKey    = "_version_"; // 版本号
static constexpr std::string_view AdminsKey     = "admins";    // 管理员
static constexpr std::string_view PlotKeyPrefix = "plot_";     // 地皮

struct AdminList {
    std::unordered_set<mce::UUID> data_;
    std::atomic_bool              isDirty{false};

    AdminList() = default;
    PLOTX_DISALLOW_COPY_AND_MOVE(AdminList);

    bool add(mce::UUID const& uuid) {
        if (data_.contains(uuid)) {
            return false;
        }
        return data_.emplace(uuid).second;
    }
    bool remove(mce::UUID const& uuid) { return data_.erase(uuid) > 0; }

    void markDirty() { isDirty = true; }
    void resetDirty() { isDirty = false; }
};

struct PlotRegistry::Impl {
    std::unique_ptr<ll::data::KeyValueDB>                      db_;     // 数据库
    AdminList                                                  admins_; // 管理员
    std::unordered_map<EncodedID, std::shared_ptr<PlotHandle>> plots_;  // 地皮
    mutable std::shared_mutex                                  mutex_;  // 读写锁

    NodeTree tree_; // 地皮树

    int getDatabaseVersion() const {
        if (!db_->has(VersionKey)) {
            return -1;
        }
        return std::stoi(*db_->get(VersionKey));
    }
    void upgradeDatabase(ll::io::Logger& logger) {
        db_->set(VersionKey, std::to_string(PlotModelVersion));
        // TODO: Implement database upgrade
    }
    void loadAdmins(ll::io::Logger& logger) {
        auto admins = db_->get(AdminsKey);
        if (!admins) {
            logger.info("No admins found in database");
            return;
        }

        auto parsed = nlohmann::json::parse(*admins);
        // TODO: v1.4.1 & v1.7.0 框架反射系统无法正常处理此容器类型反射，修复后迁移到反射系统
        // https://github.com/LiteLDev/LeviLamina/issues/1764
        // reflection::json2struct(admins_, parsed);
        admins_.data_.clear();
        admins_.data_.reserve(parsed.size());
        for (auto const& rawUuid : parsed) {
            auto const& str = static_cast<std::string const&>(rawUuid);
            if (auto uuid = mce::UUID::fromString(str); uuid != mce::UUID::EMPTY()) {
                admins_.data_.emplace(uuid);
            } else {
                logger.warn("Invalid admin UUID: {}", str);
            }
        }

        logger.info("Loaded {} admins from database", admins_.data_.size());
    }
    void loadPlots(ll::io::Logger& logger) {
        for (auto const& [k, v] : db_->iter()) {
            if (!k.starts_with(PlotKeyPrefix)) {
                continue;
            }

            auto json     = nlohmann::json::parse(v);
            auto expected = PlotHandle::load(json);
            if (!expected) {
                logger.error("Failed to load plot from database: '{}' => {}", k, expected.error().message());
                continue;
            }
            auto ptr = expected.value();
            assert(ptr != nullptr);
            plots_.emplace(IntEncoder::encode(ptr->getCoord().x, ptr->getCoord().z), std::move(ptr));
        }

        logger.info("Loaded {} plots from database", plots_.size());
    }

    bool isAdmin(mce::UUID const& uuid) const { return admins_.data_.contains(uuid); }
    bool addAdmin(mce::UUID const& uuid) {
        if (!admins_.add(uuid)) {
            return false;
        }
        admins_.markDirty();
        return true;
    }
    bool removeAdmin(mce::UUID const& uuid) {
        if (!admins_.remove(uuid)) {
            return false;
        }
        admins_.markDirty();
        return true;
    }

    bool hasPlot(int x, int z) const { return plots_.contains(IntEncoder::encode(x, z)); }
    bool addPlot(std::shared_ptr<PlotHandle> const& handle) {
        auto& coord = handle->getCoord();
        if (!handle || hasPlot(coord.x, coord.z)) {
            return false;
        }
        if (plots_.emplace(IntEncoder::encode(coord.x, coord.z), handle).second) {
            handle->markDirty();
            tree_.insert(coord.x, coord.z);
            return true;
        }
        return false;
    }
    bool removePlot(int x, int z) {
        auto it = plots_.find(IntEncoder::encode(x, z));
        if (it == plots_.end()) {
            return false;
        }
        plots_.erase(it);
        tree_.remove(x, z);
        return true;
    }

    std::string buildStorageKey(std::shared_ptr<PlotHandle> const& handle) {
        return fmt::format("{}{},{}", PlotKeyPrefix, handle->getCoord().x, handle->getCoord().z);
    }
    void saveDirtyPlot() {
        for (auto const& [id, plot] : plots_) {
            if (!plot->isDirty()) {
                continue;
            }
            auto json = plot->toJson();
            if (this->db_->set(buildStorageKey(plot), json->dump())) {
                plot->resetDirty();
            }
        }
    }
    void saveDirtyAdmins() {
        if (!admins_.isDirty) {
            return;
        }
        auto j = reflection::struct2json(admins_.data_);
        if (db_->set(AdminsKey, j->dump())) {
            admins_.resetDirty();
        }
    }

    std::shared_ptr<std::atomic_bool>             quit{nullptr};
    std::shared_ptr<ll::coro::InterruptableSleep> sleep{nullptr};

    void launchSaveTask(ll::thread::ThreadPoolExecutor& thread_pool_executor) {
        quit  = std::make_shared<std::atomic_bool>(false);
        sleep = std::make_shared<ll::coro::InterruptableSleep>();
        ll::coro::keepThis([q = quit, s = sleep, this]() -> ll::coro::CoroTask<> {
            while (!q->load()) {
                s->sleepFor(std::chrono::minutes{2});
                if (q->load()) {
                    break;
                }
                std::unique_lock lock{mutex_};
                saveDirtyPlot();
                saveDirtyAdmins();
            }
            co_return;
        }).launch(thread_pool_executor);
    }
};


PlotRegistry::PlotRegistry(PlotX& plotx) : impl_(std::make_unique<Impl>()) {
    auto& logger = plotx.getSelf().getLogger();
    auto  dir    = plotx.getDatabasePath();
    bool  isNew  = !std::filesystem::exists(dir);

    std::unique_lock lock{impl_->mutex_};
    impl_->db_ = std::make_unique<ll::data::KeyValueDB>(dir);
    if (isNew) {
        impl_->db_->set(VersionKey, std::to_string(PlotModelVersion));
    }

    if (!impl_->db_->has(VersionKey)) {
        throw std::runtime_error("PlotRegistry: Invalid database version");
    }

    auto version = impl_->getDatabaseVersion();
    if (version > PlotModelVersion) {
        throw std::runtime_error("PlotRegistry: Database version is newer than supported");
    }

    if (version < PlotModelVersion) {
        impl_->upgradeDatabase(logger);
    }

    impl_->loadAdmins(logger);
    impl_->loadPlots(logger);
    impl_->launchSaveTask(plotx.getThreadPool());
}

PlotRegistry::~PlotRegistry() {
    impl_->quit->store(true);
    impl_->sleep->interrupt(true);
    {
        std::unique_lock l{impl_->mutex_};
        impl_->saveDirtyPlot();
        impl_->saveDirtyAdmins();
    }
}

bool PlotRegistry::isAdmin(mce::UUID const& uuid) const {
    std::shared_lock lock{impl_->mutex_};
    return impl_->isAdmin(uuid);
}
void PlotRegistry::addAdmin(mce::UUID const& uuid) {
    std::unique_lock lock{impl_->mutex_};
    impl_->addAdmin(uuid);
}
void PlotRegistry::removeAdmin(mce::UUID const& uuid) {
    std::unique_lock lock{impl_->mutex_};
    impl_->removeAdmin(uuid);
}

bool PlotRegistry::hasPlot(int x, int z) const {
    std::shared_lock lock{impl_->mutex_};
    return impl_->hasPlot(x, z);
}
bool PlotRegistry::hasPlot(PlotCoord const& coord) const {
    if (!coord.isValid()) {
        return false;
    }
    return hasPlot(coord.x, coord.z);
}
std::shared_ptr<PlotHandle> PlotRegistry::getPlot(int x, int z) const {
    std::shared_lock lock{impl_->mutex_};
    auto             iter = impl_->plots_.find(IntEncoder::encode(x, z));
    if (iter != impl_->plots_.end()) {
        return iter->second;
    }
    return nullptr;
}
std::shared_ptr<PlotHandle> PlotRegistry::getPlot(PlotCoord const& coord) const {
    if (!coord.isValid()) {
        return nullptr;
    }
    return getPlot(coord.x, coord.z);
}
bool PlotRegistry::addPlot(std::shared_ptr<PlotHandle> handle) {
    std::unique_lock lock{impl_->mutex_};
    return impl_->addPlot(handle);
}
bool PlotRegistry::removePlot(int x, int z) {
    std::unique_lock lock{impl_->mutex_};
    return impl_->removePlot(x, z);
}
bool PlotRegistry::removePlot(PlotCoord const& coord) {
    if (!coord.isValid()) {
        return false;
    }
    return removePlot(coord.x, coord.z);
}
bool PlotRegistry::removePlot(std::shared_ptr<PlotHandle> const& handle) { return removePlot(handle->getCoord()); }

std::vector<std::shared_ptr<PlotHandle>> PlotRegistry::getPlots(mce::UUID const& owner) const {
    std::shared_lock lock{impl_->mutex_};

    std::vector<std::shared_ptr<PlotHandle>> result{};
    for (auto const& [k, v] : impl_->plots_) {
        if (v->isOwner(owner)) {
            result.emplace_back(v);
        }
    }
    return result;
}
std::vector<std::shared_ptr<PlotHandle>> PlotRegistry::getSellingPlots() const {
    std::shared_lock lock{impl_->mutex_};

    std::vector<std::shared_ptr<PlotHandle>> result{};
    for (auto const& [k, v] : impl_->plots_) {
        if (v->isForSale()) {
            result.emplace_back(v);
        }
    }
    return result;
}

std::optional<PlotCoord> PlotRegistry::findUnownedPlot(int x, int z) const {
    std::shared_lock lock{impl_->mutex_};
    if (auto node = impl_->tree_.findNearestUnmarkedNodeFromRoot({x, z})) {
        return PlotCoord{node->x, node->z};
    }
    return std::nullopt;
}


} // namespace plotx