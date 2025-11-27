#include "PlotHandle.hpp"
#include "mc/platform/UUID.h"
#include "model/StorageModel.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "plotx/infra/DirtyCounter.hpp"
#include "plotx/infra/Reflection.hpp"
#include "plotx/math/PlotCoord.hpp"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>


namespace plotx {

struct PlotHandle::Impl {
    PlotModel    data_{};
    DirtyCounter dirty_{};
    IdAllocator  commentId_{};

    // 缓存
    PlotCoord                        coord_{};
    mutable std::optional<mce::UUID> owner_{};
};


PlotHandle::PlotHandle() : impl(std::make_unique<Impl>()) {};
PlotHandle::PlotHandle(PlotModel record) : impl(std::make_unique<Impl>()) {
    impl->data_  = std::move(record);

    if (!impl->data_.comments_.empty()) {
        // 初始化评论ID分配器
        uint32_t max = 0;
        for (auto const& comment : impl->data_.comments_) {
            if (comment.id_ > max) {
                max = comment.id_;
            }
        }
        impl->commentId_.reset(++max);
    }
}

PlotHandle::~PlotHandle() = default;

DirtyCounter& PlotHandle::getDirtyCounter() { return impl->dirty_; }

DirtyCounter const& PlotHandle::getDirtyCounter() const { return impl->dirty_; }

PlotCoord const& PlotHandle::getCoord() const { return impl->coord_; }

mce::UUID const& PlotHandle::getOwner() const {
    if (impl->owner_.has_value()) {
        return *impl->owner_;
    }
    impl->owner_ = mce::UUID::fromString(impl->data_.owner_);
    return *impl->owner_;
}

void PlotHandle::setOwner(mce::UUID const& owner) {
    impl->owner_       = owner; // cache the value
    impl->data_.owner_ = owner.asString();
    impl->dirty_.inc();
}

std::string const& PlotHandle::getName() const { return impl->data_.name_; }

void PlotHandle::setName(std::string const& name) {
    impl->data_.name_ = name;
    impl->dirty_.inc();
}

bool PlotHandle::isSale() const { return impl->data_.isSale_; }

void PlotHandle::setSale(bool sale) {
    impl->data_.isSale_ = sale;
    impl->dirty_.inc();
}

int PlotHandle::getPrice() const { return impl->data_.price_; }

void PlotHandle::setPrice(int price) {
    impl->data_.price_ = price;
    impl->dirty_.inc();
}

bool PlotHandle::isMember(mce::UUID const& member) const {
    return std::find(impl->data_.members_.begin(), impl->data_.members_.end(), member.asString())
        != impl->data_.members_.end();
}

std::vector<std::string> const& PlotHandle::getMembers() const { return impl->data_.members_; }

void PlotHandle::addMember(mce::UUID const& member) {
    if (isMember(member)) {
        return;
    }
    impl->data_.members_.push_back(member.asString());
    impl->dirty_.inc();
}

void PlotHandle::removeMember(mce::UUID const& member) {
    auto it = std::find(impl->data_.members_.begin(), impl->data_.members_.end(), member.asString());
    if (it == impl->data_.members_.end()) {
        return;
    }
    impl->data_.members_.erase(it);
    impl->dirty_.inc();
}

std::vector<CommentModel> const& PlotHandle::getComments() const { return impl->data_.comments_; }

std::vector<CommentModel> PlotHandle::getComments(mce::UUID const& author) const {
    auto str = author.asString();

    std::vector<CommentModel> result;
    std::copy_if(
        impl->data_.comments_.begin(),
        impl->data_.comments_.end(),
        std::back_inserter(result),
        [&str](auto const& c) { return c.author_ == str; }
    );
    return result;
}

std::optional<CommentModel> PlotHandle::getComment(CommentID id) const {
    auto it = std::find_if(impl->data_.comments_.begin(), impl->data_.comments_.end(), [&id](auto const& c) {
        return c.id_ == id;
    });
    if (it == impl->data_.comments_.end()) {
        return std::nullopt;
    }
    return *it;
}

CommentID PlotHandle::addComment(mce::UUID const& author, std::string const& content) {
    auto id = impl->commentId_.next();
    impl->data_.comments_.emplace_back(
        id,
        author.asString(),
        content,
        "time" // TODO: get current time
    );
    impl->dirty_.inc();
    return id;
}

void PlotHandle::removeComment(CommentID id) {
    auto it = std::find_if(impl->data_.comments_.begin(), impl->data_.comments_.end(), [&id](auto const& c) {
        return c.id_ == id;
    });
    if (it == impl->data_.comments_.end()) {
        return;
    }
    impl->data_.comments_.erase(it);
    impl->dirty_.inc();
}


// helper
nlohmann::json PlotHandle::dump() const { return reflection::struct2json(impl->data_); }

std::shared_ptr<PlotHandle> PlotHandle::load(nlohmann::json& json) {
    auto record = PlotModel{};
    reflection::json2structVersionPatch(json, record);
    return PlotHandle::make(std::move(record));
}

} // namespace plotx