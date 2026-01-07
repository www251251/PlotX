#include "PlotGenerator.hpp"
#include "plotx/core/Config.hpp"

#include "mc/world/level/BlockPos.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/BlockVolume.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/chunk/ChunkState.h"
#include "mc/world/level/chunk/LevelChunk.h"
#include "mc/world/level/levelgen/WorldGenerator.h"
#include "mc/world/level/levelgen/v1/ChunkLocalNoiseCache.h"
#include <mc/world/level/biome/registry/BiomeRegistry.h>
#include <mc/world/level/block/VanillaBlockTypeIds.h>
#include <mc/world/level/dimension/Dimension.h>

#include <cstdlib>

// #include <mc/world/level/biome/source/FixedBiomeSource.h>
#include "minecraft/FixedBiomeSource.h"

namespace plotx::world {


PlotGenerator::ThreadData::ThreadData(PlotGenerator& generator) : buffer_() {
    buffer_.fill(generator.airBlock_);

    helper::fillLayer(buffer_, generator.bedrockBlock_, 0);
    helper::fillRange(buffer_, generator.fillBlock_, 1, generator.generatorHeight_);
    helper::fillLayer(buffer_, generator.surfaceBlock_, generator.generatorHeight_);

    volume_                 = generator.mPrototype;
    volume_.mHeight         = helper::WorldTotalHeight;
    volume_.mBlocks->mBegin = &*buffer_.begin();
    volume_.mBlocks->mEnd   = &*buffer_.end();
}

PlotGenerator::PlotGenerator(Dimension& dimension, uint seed, Json::Value const& options)
: FlatWorldGenerator(dimension, seed, options) {
    auto const& cfg = gConfig_.generator;
    if (cfg.generatorHeight < helper::WorldMinHeight || cfg.generatorHeight > helper::WorldMaxHeight) {
        throw std::runtime_error(
            "generatorHeight must be between " + std::to_string(helper::WorldMinHeight) + " and "
            + std::to_string(helper::WorldMaxHeight)
        );
    }

    mBiome = mLevel->getBiomeRegistry().lookupByName(cfg.biome);
    if (!mBiome) {
        throw std::runtime_error("Unknown biome: " + cfg.biome);
    }
    mBiomeSource = std::make_unique<FixedBiomeSource>(*mBiome);

    auto& registry = BlockTypeRegistry::get();
    airBlock_      = &registry.getDefaultBlockState("minecraft:air");
    bedrockBlock_  = &registry.getDefaultBlockState(VanillaBlockTypeIds::Bedrock());
    surfaceBlock_  = &registry.getDefaultBlockState(cfg.surfaceBlock.c_str());
    fillBlock_     = &registry.getDefaultBlockState(cfg.fillBlock.c_str());
    borderBlock_   = &registry.getDefaultBlockState(cfg.borderBlock.c_str());
    roadBlock_     = &registry.getDefaultBlockState(cfg.roadBlock.c_str());

    if (!airBlock_ || !bedrockBlock_ || !surfaceBlock_ || !fillBlock_ || !borderBlock_ || !roadBlock_) {
        throw std::runtime_error("Failed to get block, world generator failed to initialize");
    }

    // 由于总高为 384(-64 ~ 320), 所以 config 的 generatorHeight 需要 offset 64
    (*const_cast<int*>(&generatorHeight_)) = cfg.generatorHeight + std::abs(helper::WorldMinHeight);
    (*const_cast<int*>(&borderHeight_))    = generatorHeight_ + 1;
    if (borderHeight_ > helper::WorldMaxHeight) {
        throw std::runtime_error(
            "borderHeight must be less than or equal to " + std::to_string(helper::WorldMaxHeight)
        );
    }
}


[[nodiscard]] inline int positiveMod(int value, int modulus) {
    int result = value % modulus;
    if (result < 0) {
        result += modulus;
    }
    return result;
}

enum class AreaType { Plot, Border, Road };
[[nodiscard]] inline AreaType classify1D(int offset, int roadL, int roadR, int borderW, int plotW) {
    int borderL = roadL;
    int plotL   = borderL + borderW;
    int plotR   = plotL + plotW;
    int borderR = plotR + borderW;
    int cellW   = borderR + roadR;

    if (offset < borderL) return AreaType::Road;
    if (offset < plotL) return AreaType::Border;
    if (offset < plotR) return AreaType::Plot;
    if (offset < borderR) return AreaType::Border;
    return AreaType::Road;
}
[[nodiscard]] inline AreaType combine2D(AreaType x, AreaType z) {
    if (x == AreaType::Road || z == AreaType::Road) {
        return AreaType::Road;
    }
    if (x == AreaType::Plot && z == AreaType::Plot) {
        return AreaType::Plot;
    }
    return AreaType::Border;
}


void PlotGenerator::loadChunk(LevelChunk& lc, bool) {
    static thread_local std::unique_ptr<ThreadData> data(new ThreadData(*this));

    helper::fillLayer(data->buffer_, surfaceBlock_, generatorHeight_);
    helper::fillLayer(data->buffer_, airBlock_, borderHeight_);

    auto const& cfg      = gConfig_.generator;
    auto const& chunkPos = lc.mPosition.get();

    int startX = chunkPos.x * 16;
    int startZ = chunkPos.z * 16;

    int roadTotalW = cfg.roadWidth;
    int roadL      = roadTotalW / 2;
    int roadR      = roadTotalW - roadL;

    int borderW = cfg.borderWidth;
    int plotW   = cfg.plotWidth;

    int cellW = roadTotalW + borderW * 2 + plotW;

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            int globalX = startX + x;
            int globalZ = startZ + z;

            int cx = positiveMod(globalX, cellW);
            int cz = positiveMod(globalZ, cellW);

            AreaType ax   = classify1D(cx, roadL, roadR, borderW, plotW);
            AreaType az   = classify1D(cz, roadL, roadR, borderW, plotW);
            AreaType area = combine2D(ax, az);

            int indexSurface = helper::getIndex(x, generatorHeight_, z);
            int indexBorder  = helper::getIndex(x, borderHeight_, z);

            switch (area) {
            case AreaType::Road:
                helper::updateBuffer(data->buffer_, indexSurface, roadBlock_);
                break;

            case AreaType::Border:
                helper::updateBuffer(data->buffer_, indexBorder, borderBlock_);
                break;

            case AreaType::Plot:
                break;
            }
        }
    }

    lc.setBlockVolume(data->volume_, 0);
    mBiomeSource->fillBiomes(lc, nullptr);
    lc.recomputeHeightMap(false);
    lc.setSaved();

    auto ing = ChunkState::Generating;
    lc.mLoadState->compare_exchange_strong(ing, ChunkState::Generated);
}


} // namespace plotx::world