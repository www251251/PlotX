#pragma once
#include "plotx/Global.hpp"
#include <cstdint>
#include <string>
#include <vector>


namespace plotx {


using CommentID = uint32_t;
struct CommentModel {
    CommentID   id_{};      // 评论id
    std::string author_{};  // 评论者
    std::string content_{}; // 评论内容
    std::string time_{};    // 评论时间
};


constexpr int PlotModelVersion = 20;
struct PlotModel {
    int                       version{PlotModelVersion};
    int                       x{}, z{};    // 地皮坐标
    std::string               owner_{};    // 地皮主人
    std::string               name_{};     // 地皮名称
    bool                      isSale_{};   // 是否正在出售
    int                       price_{};    // 出售价格
    std::vector<std::string>  members_{};  // 地皮成员
    std::vector<CommentModel> comments_{}; // 评论
};

STATIC_ASSERT_AGGREGATE(CommentModel);
STATIC_ASSERT_AGGREGATE(PlotModel);


} // namespace plotx