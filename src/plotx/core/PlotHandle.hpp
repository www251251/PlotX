#pragma once
#include "mc/platform/UUID.h"
#include "model/StorageModel.hpp"
#include "nlohmann/json_fwd.hpp"
#include "plotx/Global.hpp"
#include "plotx/infra/DirtyCounter.hpp"
#include "plotx/infra/IdAllocator.hpp"
#include "plotx/math/PlotCoord.hpp"
#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace plotx {


/**
 * @brief 地皮句柄，对地皮数据的操作接口
 */
class PlotHandle {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    PLOTX_DISALLOW_COPY(PlotHandle);

    PXAPI explicit PlotHandle();
    PXAPI explicit PlotHandle(PlotModel record);
    PXAPI ~PlotHandle();

    template <typename... Args>
        requires std::constructible_from<PlotHandle, Args...>
    static std::shared_ptr<PlotHandle> make(Args&&... args) {
        return std::make_shared<PlotHandle>(std::forward<Args>(args)...);
    }

    PXAPI DirtyCounter& getDirtyCounter();

    PXAPI DirtyCounter const& getDirtyCounter() const;

    PXAPI PlotCoord const& getCoord() const;

    PXNDAPI mce::UUID const& getOwner() const;

    PXAPI void setOwner(mce::UUID const& owner);

    PXNDAPI std::string const& getName() const;

    PXAPI void setName(std::string const& name);

    PXNDAPI bool isSale() const;

    PXAPI void setSale(bool sale);

    PXNDAPI int getPrice() const;

    PXAPI void setPrice(int price);

    PXNDAPI bool isMember(mce::UUID const& member) const;

    PXNDAPI std::vector<std::string> const& getMembers() const;

    PXAPI void addMember(mce::UUID const& member);

    PXAPI void removeMember(mce::UUID const& member);

    PXAPI std::vector<CommentModel> const& getComments() const;

    PXAPI std::vector<CommentModel> getComments(mce::UUID const& author) const;

    PXAPI std::optional<CommentModel> getComment(CommentID id) const;

    PXAPI CommentID addComment(mce::UUID const& author, std::string const& content);

    PXAPI void removeComment(CommentID id);

    PXNDAPI bool isMergedMultiPlot() const; // 是否为合并多块地皮

    PXNDAPI int getMergeCounter() const; // 合并地皮次数

    // helper
    PXNDAPI nlohmann::json dump() const;

    PXNDAPI static std::shared_ptr<PlotHandle> load(nlohmann::json& json);
};


} // namespace plotx