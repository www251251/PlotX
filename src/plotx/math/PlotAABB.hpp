#pragma once
#include "plotx/Global.hpp"
#include <functional>
#include <vector>

#include "mc/world/level/BlockPos.h"

class Block;
namespace plotx {


class PlotAABB {
public:
    BlockPos min{}, max{};

    PlotAABB() = default;
    PXAPI explicit PlotAABB(BlockPos const& min, BlockPos const& max);

    PXNDAPI std::string toString() const;

    PXNDAPI BlockPos&       getMin();
    PXNDAPI BlockPos&       getMax();
    PXNDAPI BlockPos const& getMin() const;
    PXNDAPI BlockPos const& getMax() const;

    /**
     * 检查一个点是否在 AABB 的边上
     */
    PXNDAPI bool isOnEdge(BlockPos const& pos) const;

    /**
     * 检查一个点是否在 AABB 内部
     */
    PXNDAPI bool contains(BlockPos const& pos) const;

    /**
     * 获取AABB的边（平面）
     * 示例（俯视视图）：
     *
     *   z ↑
     *     │  ┌───────┐
     *     │  │       │
     *     │  │       │
     *     │  └───────┘
     *     └───────────────→ x
     */
    PXNDAPI std::vector<BlockPos> getEdges(int y) const;

    /**
     * 获取平面矩形的顶点
     * @param closure 是否闭合
     */
    PXNDAPI std::vector<BlockPos> getVertices(bool closure = true) const;

    /**
     * 遍历AABB内的所有方块
     */
    PXAPI void forEach(std::function<bool(BlockPos const&)> const& fn) const;

    /**
     * 遍历AABB的某一层
     */
    PXAPI void forEachLayer(int y, std::function<bool(BlockPos const&)> const& fn) const;

    /**
     * 遍历 AABB 的边 (平面顶点遍历)
     */
    PXAPI void forEachEdge(std::function<bool(BlockPos const&)> const& fn) const;

    /**
     * 遍历 AABB 的边(指定层)
     */
    PXAPI void forEachEdgeLayer(int y, std::function<bool(BlockPos const&)> const& fn) const;

    /**
     * 填充AABB的某一层
     */
    PXAPI bool fillLayer(int y, Block const& block) const;

    /**
     * 填充指定层的边
     */
    PXAPI bool fillEdgeLayer(int y, Block const& block) const;

    PXAPI bool operator==(PlotAABB const& other) const;


protected:
    static void implForEach(BlockPos const& min, BlockPos const& max, std::function<bool(BlockPos const&)> const& fn);

    static void
    implForEachLayer(BlockPos const& min, BlockPos const& max, int y, std::function<bool(BlockPos const&)> const& fn);
};


} // namespace plotx