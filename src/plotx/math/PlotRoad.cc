#include "PlotRoad.hpp"
#include "fmt/color.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/dimension/Dimension.h"
#include "plotx/PlotX.hpp"
#include "plotx/generator/Helper.hpp"
#include "plotx/infra/Config.hpp"
#include "plotx/math/PlotAABB.hpp"
#include <cmath>
#include <mc/world/level/block/BlockChangeContext.h>


namespace plotx {


PlotRoad::PlotRoad(int x, int z, bool isTransverse) : x(x), z(z), isTransverse_(isTransverse) {
    auto const& cfg = gConfig_.generator;

    int total = cfg.plotWidth + cfg.roadWidth;
    if (isTransverse_) {
        // 横向道路
        min           = BlockPos{x * total + cfg.plotWidth, generator::WorldMinHeight, z * total};
        max           = BlockPos{min.x + cfg.roadWidth - 1, generator::WorldMaxHeight, min.z + cfg.plotWidth - 1};
        isTransverse_ = true;
    } else {
        // 纵向道路
        min           = BlockPos{x * total, generator::WorldMinHeight, z * total + cfg.plotWidth};
        max           = BlockPos{min.x + cfg.plotWidth - 1, generator::WorldMaxHeight, min.z + cfg.plotWidth - 1};
        isTransverse_ = false;
    }
    valid_ = true;
}

PlotRoad::PlotRoad(BlockPos const& pos) {
    auto const& cfg = gConfig_.generator;

    int total = cfg.plotWidth + cfg.roadWidth;

    auto dx = static_cast<double>(pos.x);
    auto dz = static_cast<double>(pos.z);

    x = static_cast<int>(std::floor(dx / total));
    z = static_cast<int>(std::floor(dz / total));

    double localX = std::fmod(dx, total);
    double localZ = std::fmod(dz, total);
    if (localX < 0) localX += total;
    if (localZ < 0) localZ += total;

    if (localX >= cfg.plotWidth && localX < total && localZ < cfg.plotWidth) {
        // 横向道路
        min           = BlockPos{x * total + cfg.plotWidth, generator::WorldMinHeight, z * total};
        max           = BlockPos{min.x + cfg.roadWidth - 1, generator::WorldMaxHeight, min.z + cfg.plotWidth - 1};
        isTransverse_ = true;
        valid_        = true;

    } else if (localZ >= cfg.plotWidth && localZ < total && localX < cfg.plotWidth) {
        // 纵向道路
        min           = BlockPos{x * total, generator::WorldMinHeight, z * total + cfg.plotWidth};
        max           = BlockPos{min.x + cfg.plotWidth - 1, generator::WorldMaxHeight, min.z + cfg.roadWidth - 1};
        isTransverse_ = false;
        valid_        = true;

    } else {
        // 不在道路上
        valid_ = false;
        x      = 0;
        z      = 0;
    }
}


bool PlotRoad::isTransverse() const { return isTransverse_; }

bool PlotRoad::isLongitudinal() const { return !isTransverse_; }

std::string PlotRoad::toString() const {
    return fmt::format("{}({}, {})\n{}", isTransverse_ ? "横向" : "纵向", x, z, PlotAABB ::toString());
}

bool PlotRoad::isValid() const { return valid_; }

void PlotRoad::removeNeighbourBorder() const {
    auto dim = ll::service::getLevel()->getDimension(PlotX::getDimensionId()).lock();
    if (!dim) {
        return;
    }
    auto& bs = dim->getBlockSourceFromMainChunkSource();

    auto const& air = BlockTypeRegistry::get().getDefaultBlockState("minecraft:air");

    int const borderHeight = gConfig_.generator.generatorHeight + 1;

    auto  vertices = getVertices(false);
    auto& v0       = vertices[0]; // 左下 min
    auto& v1       = vertices[1]; // 左上
    auto& v2       = vertices[2]; // 右上 max
    auto& v3       = vertices[3]; // 右下

    auto ctx    = BlockChangeContext{};
    auto handle = [&](BlockPos const& pos) {
        bs.setBlock(pos, air, 3, nullptr, ctx);
        return true;
    };

    if (isTransverse_) {
        // 横向
        v0.x--;
        v1.x++;
        v2.x++;
        v3.x--;
        implForEachLayer(v0, v3, borderHeight, handle);
        implForEachLayer(v1, v2, borderHeight, handle);

    } else {
        // 纵向
        v0.z--;
        v1.z--;
        v2.z++;
        v3.z++;
        implForEachLayer(v0, v1, borderHeight, handle);
        implForEachLayer(v3, v2, borderHeight, handle);
    }
}

bool PlotRoad::operator==(PlotRoad const& other) const {
    return x == other.x && z == other.z && isTransverse_ == other.isTransverse_ && PlotAABB::operator==(other);
}


} // namespace plotx