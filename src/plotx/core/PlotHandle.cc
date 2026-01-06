#include "PlotHandle.hpp"
#include "model/StorageModel.hpp"
#include "plotx/infra/DirtyCounter.hpp"
#include "plotx/infra/IdAllocator.hpp"
#include "plotx/infra/Reflection.hpp"
#include "plotx/math/PlotCoord.hpp"

#include "mc/platform/UUID.h"

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

#include "plotx/utils/TimeUtils.hpp"

#include <algorithm>
#include <perm_core/model/PermRole.hpp>
#include <string>
#include <utility>
#include <vector>

namespace plotx {

struct PlotHandle::Impl {
    PlotModel    data_{};
    DirtyCounter dirty_{};
    IdAllocator  commentId_{};

    // 缓存
    PlotCoord                        coordCache_{};
    mutable std::optional<mce::UUID> ownerCache_{};

    void initIdAllocator() {
        if (data_.comments_.empty()) {
            return;
        }
        uint32_t max = 0;
        for (auto const& comment : data_.comments_) {
            if (comment.id_ > max) {
                max = comment.id_;
            }
        }
        commentId_.reset(++max);
    }
    void initCache() {
        coordCache_ = PlotCoord{data_.x, data_.z};
        if (!ownerCache_) {
            if (auto uuid = mce::UUID::fromString(data_.owner_); uuid != mce::UUID::EMPTY()) {
                ownerCache_ = uuid;
            }
        }
    }
    void init() {
        initIdAllocator();
        initCache();
    }
};


PlotHandle::PlotHandle(PlotModel record) : impl(std::make_unique<Impl>()) {
    impl->data_ = std::move(record);
    impl->init();
}
PlotHandle::PlotHandle(int x, int z, mce::UUID const& owner) : impl(std::make_unique<Impl>()) {
    impl->data_.x      = x;
    impl->data_.z      = z;
    impl->data_.owner_ = owner.asString();
    impl->ownerCache_  = owner;
    impl->initCache();
}
PlotHandle::PlotHandle(PlotCoord const& coord, mce::UUID const& owner) : PlotHandle(coord.x, coord.z, owner) {}

PlotHandle::~PlotHandle() = default;

void PlotHandle::markDirty() { impl->dirty_.inc(); }

permc::PermRole PlotHandle::getPlayerRole(mce::UUID const& player) const {
    if (isOwner(player)) {
        return permc::PermRole::Owner;
    }
    if (isMember(player)) {
        return permc::PermRole::Member;
    }
    return permc::PermRole::Guest;
}
permc::PermTable&       PlotHandle::getPermTable() { return impl->data_.ptable_; }
permc::PermTable const& PlotHandle::getPermTable() const { return impl->data_.ptable_; }
void                    PlotHandle::setPermTable(permc::PermTable table) {
    impl->data_.ptable_ = std::move(table);
    markDirty();
}

PlotCoord const& PlotHandle::getCoord() const { return impl->coordCache_; }
bool             PlotHandle::isOwner(mce::UUID const& uuid) const { return uuid == getOwner(); }

mce::UUID const& PlotHandle::getOwner() const {
    if (impl->ownerCache_.has_value()) [[likely]] {
        return *impl->ownerCache_;
    }
    impl->ownerCache_ = mce::UUID::fromString(impl->data_.owner_);
    return *impl->ownerCache_;
}

void PlotHandle::setOwner(mce::UUID const& owner) {
    impl->ownerCache_  = owner; // cache the value
    impl->data_.owner_ = owner.asString();
    markDirty();
}

std::string const& PlotHandle::getName() const { return impl->data_.name_; }

void PlotHandle::setName(std::string name) {
    impl->data_.name_ = std::move(name);
    markDirty();
}

bool PlotHandle::isForSale() const { return impl->data_.sellingPrice_ != NotForSale; }

int PlotHandle::getSellingPrice() const { return impl->data_.sellingPrice_; }

void PlotHandle::setSellingPrice(int price) {
    if (price < NotForSale) {
        throw std::invalid_argument("sellingPrice must be >= 0, or NotForSale (-1) if not for sale");
    }
    impl->data_.sellingPrice_ = price;
    markDirty();
}

bool PlotHandle::isMember(mce::UUID const& member) const {
    return std::ranges::find(impl->data_.members_, member.asString()) == impl->data_.members_.end();
}

std::vector<std::string> const& PlotHandle::getMembers() const { return impl->data_.members_; }

void PlotHandle::addMember(mce::UUID const& member) {
    if (isMember(member)) {
        return;
    }
    impl->data_.members_.push_back(member.asString());
    markDirty();
}

void PlotHandle::removeMember(mce::UUID const& member) {
    auto it = std::ranges::find(impl->data_.members_, member.asString());
    if (it == impl->data_.members_.end()) {
        return;
    }
    impl->data_.members_.erase(it);
    markDirty();
}

std::vector<CommentModel> const& PlotHandle::getComments() const { return impl->data_.comments_; }

std::vector<CommentModel> PlotHandle::getComments(mce::UUID const& author) const {
    auto str = author.asString();

    std::vector<CommentModel> result;
    std::ranges::copy_if(impl->data_.comments_, std::back_inserter(result), [&str](auto const& c) {
        return c.author_ == str;
    });
    return result;
}

std::optional<CommentModel> PlotHandle::getComment(CommentID id) const {
    auto it = std::ranges::find_if(impl->data_.comments_, [&id](auto const& c) { return c.id_ == id; });
    if (it == impl->data_.comments_.end()) {
        return std::nullopt;
    }
    return *it;
}

CommentID PlotHandle::addComment(mce::UUID const& author, std::string const& content) {
    auto id = impl->commentId_.next();
    impl->data_.comments_.emplace_back(id, author.asString(), content, time_utils::format(time_utils::getLocalTime()));
    markDirty();
    return id;
}
bool PlotHandle::updateCommentContent(CommentID id, std::string content) {
    auto it = std::ranges::find_if(impl->data_.comments_, [&id](auto const& c) { return c.id_ == id; });
    if (it == impl->data_.comments_.end()) {
        return false;
    }
    it->content_ = std::move(content);
    markDirty();
    return true;
}

void PlotHandle::removeComment(CommentID id) {
    auto it = std::ranges::find_if(impl->data_.comments_, [&id](auto const& c) { return c.id_ == id; });
    if (it == impl->data_.comments_.end()) {
        return;
    }
    impl->data_.comments_.erase(it);
    markDirty();
}


// helper
ll::Expected<nlohmann::json>              PlotHandle::toJson() const { return reflection::struct2json(impl->data_); }
ll::Expected<std::shared_ptr<PlotHandle>> PlotHandle::load(nlohmann::json& json) {
    auto record = PlotModel{};
    reflection::json2structVersionPatch(json, record);
    return make(std::move(record));
}

} // namespace plotx