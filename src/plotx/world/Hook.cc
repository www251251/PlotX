#ifdef PLOTX_OVERWORLD
#include "ll/api/memory/Hook.h"

#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/OverworldDimension.h"
#include "mc/world/level/levelgen/WorldGenerator.h"
#include "mc/world/level/levelgen/structure/registry/StructureSetRegistry.h"
#include "mc/world/level/storage/LevelData.h"
#include "plotx/world/generator/PlotGenerator.hpp"

#include <memory>

#include "minecraft/FixedBiomeSource.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    OverworldDimensionCreateGeneratorHook,
    ll::memory::HookPriority::Normal,
    OverworldDimension,
    &OverworldDimension::$createGenerator,
    std::unique_ptr<WorldGenerator>,
    br::worldgen::StructureSetRegistry const& /* structureSetRegistry */
) {
    mSeaLevel = -61;

    auto gen =
        std::make_unique<plotx::world::PlotGenerator>(*this, mLevel.getSeed(), mLevel.getLevelData().mFlatWorldOptions);

    return std::move(gen);
}

#endif