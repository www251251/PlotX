#include "PlotAABB.hpp"
#include "fmt/format.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/Dimension.h"
#include "plotx/PlotX.hpp"
#include <cstddef>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/block/BlockChangeContext.h>
#include <vector>


namespace plotx {


PlotAABB::PlotAABB(BlockPos const& min, BlockPos const& max) : min(min), max(max) {}

std::string PlotAABB::toString() const { return fmt::format("PlotAABB({}, {})", min.toString(), max.toString()); }

BlockPos&       PlotAABB::getMin() { return min; }
BlockPos&       PlotAABB::getMax() { return max; }
BlockPos const& PlotAABB::getMin() const { return min; }
BlockPos const& PlotAABB::getMax() const { return max; }

bool PlotAABB::isOnEdge(BlockPos const& pos) const {
    bool onXEdge = pos.x == min.x || pos.x == max.x;
    bool onZEdge = pos.z == min.z || pos.z == max.z;

    bool withinXRange = pos.x >= min.x && pos.x <= max.x;
    bool withinZRange = pos.z >= min.z && pos.z <= max.z;

    return (onXEdge && withinXRange) || (onZEdge && withinZRange);
}

bool PlotAABB::contains(BlockPos const& pos) const {
    return pos.x >= min.x && pos.x <= max.x && pos.z >= min.z && pos.z <= max.z;
}

std::vector<BlockPos> PlotAABB::getEdges(int y) const {
    std::vector<BlockPos> edges;
    for (int x = min.x; x <= max.x; ++x) {
        edges.emplace_back(x, y, min.z);
        edges.emplace_back(x, y, max.z);
    }
    for (int z = min.z + 1; z < max.z; ++z) {
        edges.emplace_back(min.x, y, z);
        edges.emplace_back(max.x, y, z);
    }
    return edges;
}

void PlotAABB::forEach(std::function<bool(BlockPos const&)> const& fn) const { implForEach(min, max, fn); }

void PlotAABB::forEachLayer(int y, std::function<bool(BlockPos const&)> const& fn) const {
    implForEachLayer(min, max, y, fn);
}

bool PlotAABB::fillLayer(int y, Block const& block) const {
    auto dim = ll::service::getLevel()->getDimension(PlotX::getDimensionId()).lock();
    if (!dim) {
        return false;
    }

    auto& bs = dim->getBlockSourceFromMainChunkSource();

    auto ctx = BlockChangeContext{};
    forEachLayer(y, [&](BlockPos const& pos) {
        bs.setBlock(pos, block, 3, nullptr, ctx);
        return true;
    });

    return true;
}

std::vector<BlockPos> PlotAABB::getVertices(bool closure) const {
    std::vector<BlockPos> vertices = {
        min, // 左下
        {max.x, min.y, min.z}, // 左上
        max, // 右上
        {min.x, max.y, max.z}, // 右下
    };
    if (closure) {
        vertices.push_back(min);
    }
    return vertices;
}

void PlotAABB::forEachEdge(std::function<bool(BlockPos const&)> const& fn) const {
    auto vertices = getVertices();

    size_t   next = 0;
    BlockPos p1, p2;
    for (auto const& v : vertices) {
        next++;
        if (next == vertices.size()) {
            next = 0;
        }

        p1 = v;
        p2 = vertices[next];

        implForEach(p1, p2, fn);
    }
}

void PlotAABB::forEachEdgeLayer(int y, std::function<bool(BlockPos const&)> const& fn) const {
    for (auto& edge : getEdges(y)) {
        if (!fn(edge)) {
            return;
        }
    }
}

bool PlotAABB::fillEdgeLayer(int y, Block const& block) const {
    auto dim = ll::service::getLevel()->getDimension(PlotX::getDimensionId()).lock();
    if (!dim) {
        return false;
    }

    auto& bs  = dim->getBlockSourceFromMainChunkSource();
    auto  ctx = BlockChangeContext{};
    forEachEdgeLayer(y, [&](BlockPos const& pos) {
        bs.setBlock(pos, block, 3, nullptr, ctx);
        return true;
    });

    return true;
}


bool PlotAABB::operator==(PlotAABB const& other) const { return other.min == min && other.max == max; }


// impl
void PlotAABB::implForEach(BlockPos const& min, BlockPos const& max, std::function<bool(BlockPos const&)> const& fn) {
    BlockPos curr = min;

    auto& x = curr.x;
    auto& y = curr.y;
    auto& z = curr.z;
    for (x = min.x; x <= max.x; x++) {
        for (z = min.z; z <= max.z; z++) {
            for (y = min.y; y <= max.y; y++) {
                if (!fn(curr)) {
                    return;
                }
            }
        }
    }
}

void PlotAABB::implForEachLayer(
    BlockPos const&                             min,
    BlockPos const&                             max,
    int                                         y,
    std::function<bool(BlockPos const&)> const& fn
) {
    BlockPos curr = min;
    curr.y        = y;

    auto& x = curr.x;
    auto& z = curr.z;
    for (x = min.x; x <= max.x; x++) {
        for (z = min.z; z <= max.z; z++) {
            if (!fn(curr)) {
                return;
            }
        }
    }
}


} // namespace plotx