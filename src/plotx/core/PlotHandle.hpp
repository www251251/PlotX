#pragma once
#include "model/StorageModel.hpp"
#include "plotx/Global.hpp"

#include "nlohmann/json_fwd.hpp"
#include "plotx/math/PlotCoord.hpp"

#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace permc {
enum class PermRole;
}
namespace mce {
class UUID;
}

namespace plotx {

class PlotCoord;

/**
 * @brief 地皮句柄，对地皮数据的操作接口
 */
class PlotHandle {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    PLOTX_DISALLOW_COPY(PlotHandle);

    PXNDAPI explicit PlotHandle(PlotModel record); // for PlotRegistry

    PXNDAPI explicit PlotHandle(int x, int z, mce::UUID const& owner);
    PXNDAPI explicit PlotHandle(PlotCoord const& coord, mce::UUID const& owner);

    PXAPI ~PlotHandle();

    template <typename... Args>
        requires std::constructible_from<PlotHandle, Args...>
    static std::shared_ptr<PlotHandle> make(Args&&... args) {
        return std::make_shared<PlotHandle>(std::forward<Args>(args)...);
    }

    /**
     * 标记数据已修改
     * @note 一般情况下，setXXX 的API内部都会自动标记数据修改，无需手动标记
     */
    PXAPI void markDirty();

    permc::PermRole         getPlayerRole(mce::UUID const& player) const;
    permc::PermTable&       getPermTable();
    permc::PermTable const& getPermTable() const;

    PXAPI void setPermTable(permc::PermTable table);

    PXAPI PlotCoord const& getCoord() const;

    PXNDAPI bool isOwner(mce::UUID const& uuid) const;

    PXNDAPI mce::UUID const& getOwner() const;

    PXAPI void setOwner(mce::UUID const& owner);

    PXNDAPI std::string const& getName() const;

    PXAPI void setName(std::string name);

    PXNDAPI bool isForSale() const;

    PXNDAPI int getSellingPrice() const;

    PXAPI void setSellingPrice(int price);

    PXNDAPI bool isMember(mce::UUID const& member) const;

    PXNDAPI std::vector<std::string> const& getMembers() const;

    PXAPI void addMember(mce::UUID const& member);

    PXAPI void removeMember(mce::UUID const& member);

    PXAPI std::vector<CommentModel> const& getComments() const;

    PXAPI std::vector<CommentModel> getComments(mce::UUID const& author) const;

    PXAPI std::optional<CommentModel> getComment(CommentID id) const;

    PXAPI CommentID addComment(mce::UUID const& author, std::string const& content);

    PXAPI bool updateCommentContent(CommentID id, std::string content);

    PXAPI void removeComment(CommentID id);

    // helper
    PXNDAPI ll::Expected<nlohmann::json> toJson() const;

    PXNDAPI static ll::Expected<std::shared_ptr<PlotHandle>> load(nlohmann::json& json);
};


} // namespace plotx