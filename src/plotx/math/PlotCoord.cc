#include "PlotCoord.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/Config.hpp"
#include "plotx/math/PlotAABB.hpp"
#include "plotx/world/Helper.hpp"

#include "fmt/format.h"
#include "ll/api/service/Bedrock.h"

#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/dimension/Dimension.h"
#include <mc/world/level/block/BlockChangeContext.h>

#include <cmath>

namespace plotx {

PlotCoord::PlotCoord(int x_, int z_) : x(x_), z(z_) {
    auto const& cfg        = gConfig_.generator;
    int         plotW      = cfg.plotWidth;
    int         borderW    = cfg.borderWidth;
    int         roadTotalW = cfg.roadWidth;

    int roadL = roadTotalW / 2;
    int roadR = roadTotalW - roadL;

    int cellW = roadL + borderW + plotW + borderW + roadR;

    min    = BlockPos{x * cellW + roadL + borderW, world::helper::WorldMinHeight, z * cellW + roadL + borderW};
    max    = BlockPos{min.x + plotW - 1, world::helper::WorldMaxHeight, min.z + plotW - 1};
    valid_ = true;
}

PlotCoord::PlotCoord(BlockPos const& pos) {
    auto const& cfg        = gConfig_.generator;
    int         plotW      = cfg.plotWidth;
    int         borderW    = cfg.borderWidth;
    int         roadTotalW = cfg.roadWidth;

    // 左右路宽分配
    int roadL = roadTotalW / 2;
    int roadR = roadTotalW - roadL;

    // 计算单元总宽
    int cellW = roadL + borderW + plotW + borderW + roadR;

    // 转 double 处理负数
    double dx = static_cast<double>(pos.x);
    double dz = static_cast<double>(pos.z);

    // 计算地皮坐标
    int cellX = static_cast<int>(std::floor(dx / cellW));
    int cellZ = static_cast<int>(std::floor(dz / cellW));

    // 计算局部坐标
    int localX = static_cast<int>(std::floor(dx)) - cellX * cellW;
    int localZ = static_cast<int>(std::floor(dz)) - cellZ * cellW;

    if (localX < 0) localX += cellW;
    if (localZ < 0) localZ += cellW;

    // plot 区间是 roadL + borderW ... roadL + borderW + plotW -1
    int plotStart = roadL + borderW;
    int plotEnd   = plotStart + plotW - 1;

    if (localX < plotStart || localX > plotEnd || localZ < plotStart || localZ > plotEnd) {
        valid_ = false;
        x      = 0;
        z      = 0;
    } else {
        x      = cellX;
        z      = cellZ;
        min    = BlockPos{cellX * cellW + plotStart, world::helper::WorldMinHeight, cellZ * cellW + plotStart};
        max    = BlockPos{min.x + plotW - 1, world::helper::WorldMaxHeight, min.z + plotW - 1};
        valid_ = true;
    }
}

std::string PlotCoord::toString() const { return fmt::format("PlotCoord({}, {})\n{}", x, z, PlotAABB::toString()); }

bool PlotCoord::isValid() const { return valid_; }

void PlotCoord::tryFixBorder() const {
    if (!valid_) {
        return;
    }

    auto& borderBlock = BlockTypeRegistry::get().getDefaultBlockState(gConfig_.generator.borderBlock.c_str());

    fillEdgeLayer(gConfig_.generator.generatorHeight + 1, borderBlock);
}

void PlotCoord::removeBorder() const {
    if (!valid_) {
        return;
    }

    auto& air = BlockTypeRegistry::get().getDefaultBlockState("minecraft:air");
    fillEdgeLayer(gConfig_.generator.generatorHeight + 1, air);
}

void PlotCoord::removeBorderCorners() const {
    if (!valid_) {
        return;
    }

    auto dim = ll::service::getLevel()->getDimension(PlotX::getDimensionId()).lock();
    if (!dim) {
        return;
    }
    auto& bs = dim->getBlockSourceFromMainChunkSource();

    auto const& air = BlockTypeRegistry::get().getDefaultBlockState("minecraft:air");

    int const borderHeight = gConfig_.generator.generatorHeight + 1;

    auto ctx = BlockChangeContext{};
    for (auto& v : getVertices(false)) {
        v.y = borderHeight;
        bs.setBlock(v, air, 3, nullptr, ctx);
    }
}

bool PlotCoord::operator==(PlotCoord const& pos) const { return pos.x == x && pos.z == z && PlotAABB::operator==(pos); }


} // namespace plotx