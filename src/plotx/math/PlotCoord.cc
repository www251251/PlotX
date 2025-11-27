#include "PlotCoord.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/generator/Helper.hpp"
#include "plotx/infra/Config.hpp"
#include "plotx/math/PlotAABB.hpp"

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

PlotCoord::PlotCoord(int x, int z) : x(x), z(z) {
    auto const& cfg = gConfig_.generator;

    int total = cfg.plotWidth + cfg.roadWidth;
    min       = BlockPos{x * total, generator::WorldMinHeight, z * total};
    max       = BlockPos{min.x + cfg.plotWidth - 1, generator::WorldMaxHeight, min.z + cfg.plotWidth - 1};
    valid_    = true;
}

PlotCoord::PlotCoord(BlockPos const& pos) {
    auto const& cfg       = gConfig_.generator;
    auto const& plotWidth = cfg.plotWidth;
    auto const& roadWidth = cfg.roadWidth;

    auto const total = cfg.plotWidth + roadWidth;

    // 转 double 处理负数
    double dx = static_cast<double>(pos.x);
    double dz = static_cast<double>(pos.z);

    // 计算地皮坐标
    x = static_cast<int>(std::floor(dx / total));
    z = static_cast<int>(std::floor(dz / total));

    // 计算局部坐标
    int localX = static_cast<int>(std::floor(dx)) % total;
    int localZ = static_cast<int>(std::floor(dz)) % total;
    if (localX < 0) localX += total;
    if (localZ < 0) localZ += total;

    if (localX >= plotWidth || localZ >= plotWidth) {
        valid_ = false;
        x      = 0;
        z      = 0;
    } else {
        min    = BlockPos{x * total, generator::WorldMinHeight, z * total};
        max    = BlockPos{min.x + plotWidth - 1, generator::WorldMaxHeight, min.z + plotWidth - 1};
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