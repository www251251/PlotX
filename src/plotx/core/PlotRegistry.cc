#include "PlotRegistry.hpp"
#include "ll/api/data/KeyValueDB.h"
#include "model/StorageModel.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/infra/IntEncoder.hpp"
#include "plotx/infra/Reflection.hpp"
#include <algorithm>
#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <string>


namespace plotx {

struct PlotRegistry::Impl {
    std::unique_ptr<ll::data::KeyValueDB>                      db_;     // 数据库
    std::unordered_set<mce::UUID>                              admins_; // 管理员
    std::unordered_map<EncodedID, std::shared_ptr<PlotHandle>> plots_;  // 地皮
    mutable std::shared_mutex                                  mutex_;  // 读写锁
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

    auto version = std::stoi(*impl_->db_->get(VersionKey));
    if (version > PlotModelVersion) {
        throw std::runtime_error("PlotRegistry: Database version is newer than supported");
    }

    if (version < PlotModelVersion) {
        _upgradeDatabase(logger);
    }

    _loadAdmins(logger);
    _loadPlots(logger);
}

PlotRegistry::~PlotRegistry() {}


void PlotRegistry::_upgradeDatabase(ll::io::Logger& logger) {
    impl_->db_->set(VersionKey, std::to_string(PlotModelVersion));
    // TODO: Implement database upgrade
}

void PlotRegistry::_loadAdmins(ll::io::Logger& logger) {
    auto admins = impl_->db_->get(AdminsKey);
    if (!admins) {
        logger.info("No admins found in database");
        return;
    }

    auto parsed = nlohmann::json::parse(*admins);
    // TODO: v1.4.1 框架反射系统无法正常处理此容器类型反射，修复后迁移到反射系统
    // https://github.com/LiteLDev/LeviLamina/issues/1764
    // reflection::json2struct(impl_->admins_, parsed);
    impl_->admins_.clear();
    impl_->admins_.reserve(parsed.size());
    for (auto const& rawUuid : parsed) {
        auto const& str = static_cast<std::string const&>(rawUuid);
        if (auto uuid = mce::UUID::fromString(str); uuid != mce::UUID::EMPTY()) {
            impl_->admins_.emplace(std::move(uuid));
        } else {
            logger.warn("Invalid admin UUID: {}", str);
        }
    }

    logger.info("Loaded {} admins from database", impl_->admins_.size());
}

void PlotRegistry::_loadPlots(ll::io::Logger& logger) {
    for (auto const& [k, v] : impl_->db_->iter()) {
        if (!k.starts_with(PlotKeyPrefix)) {
            continue;
        }

        auto json = nlohmann::json::parse(v);
        auto ptr  = PlotHandle::load(json);
        if (!ptr) {
            logger.error("Failed to load plot from database: {}", k);
            continue;
        }

        impl_->plots_.emplace(IntEncoder::encode(ptr->getCoord().x, ptr->getCoord().z), std::move(ptr));
    }

    logger.info("Loaded {} plots from database", impl_->plots_.size());
}


bool PlotRegistry::isAdmin(mce::UUID const& uuid) const {
    std::shared_lock lock{impl_->mutex_};
    return impl_->admins_.contains(uuid);
}

void PlotRegistry::addAdmin(mce::UUID const& uuid) {
    if (isAdmin(uuid)) {
        return;
    }
    std::unique_lock lock{impl_->mutex_};
    impl_->admins_.emplace(uuid);
}
void PlotRegistry::removeAdmin(mce::UUID const& uuid) {
    if (!isAdmin(uuid)) {
        return;
    }
    std::unique_lock lock{impl_->mutex_};
    impl_->admins_.erase(uuid);
}


} // namespace plotx