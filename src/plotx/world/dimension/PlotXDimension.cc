#include "PlotXDimension.hpp"
#include "plotx/world/Helper.hpp"

#include "mc/common/Brightness.h"
#include "mc/common/BrightnessPair.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/DimensionConversionData.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/LevelSeed64.h"
#include "mc/world/level/chunk/vanilla_level_chunk_upgrade/VanillaLevelChunkUpgrade.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/dimension/DimensionArguments.h"
#include "mc/world/level/dimension/DimensionBrightnessRamp.h"
#include "mc/world/level/dimension/DimensionHeightRange.h"
#include "mc/world/level/dimension/OverworldBrightnessRamp.h"
#include "mc/world/level/dimension/VanillaDimensions.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"
#include "mc/world/level/levelgen/structure/StructureFeatureRegistry.h"
#include "mc/world/level/levelgen/structure/VillageFeature.h"
#include "mc/world/level/levelgen/structure/registry/StructureSetRegistry.h"
#include "mc/world/level/levelgen/v2/ChunkGeneratorStructureState.h"
#include "mc/world/level/storage/LevelData.h"
#include "mc/world/level/storage/WorldTemplateLevelData.h"
#include "plotx/world/generator/PlotGenerator.hpp"

#include "minecraft/FixedBiomeSource.h"
#include <utility>

namespace plotx::world {


PlotXDimension::PlotXDimension(std::string const& name, more_dimensions::DimensionFactoryInfo const& info)
: Dimension(
      DimensionArguments{
          std::move(info.arguments),
          info.dimId,
          {helper::WorldMinHeight, helper::WorldMaxHeight},
          name
}
  ) {
    mDefaultBrightness->sky  = Brightness::MAX();
    mSeaLevel                = -61;
    mHasWeather              = true;
    mDimensionBrightnessRamp = std::make_unique<OverworldBrightnessRamp>();
    mDimensionBrightnessRamp->buildBrightnessRamp();
}

CompoundTag PlotXDimension::generateNewData() { return {}; }

std::unique_ptr<WorldGenerator>
PlotXDimension::createGenerator(br::worldgen::StructureSetRegistry const& /* structureSetRegistry */) {
    auto  seed      = mLevel.getSeed();
    auto& levelData = mLevel.getLevelData();

    auto generator = std::make_unique<PlotGenerator>(*this, seed, levelData.mFlatWorldOptions);

    return std::move(generator);
}

void PlotXDimension::upgradeLevelChunk(ChunkSource& cs, LevelChunk& lc, LevelChunk& generatedChunk) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkViaMetaData(lc, generatedChunk, blockSource);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkLegacy(lc, blockSource);
}

void PlotXDimension::fixWallChunk(ChunkSource& cs, LevelChunk& lc) {
    auto blockSource = BlockSource(static_cast<Level&>(mLevel), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::fixWallChunk(lc, blockSource);
}

bool PlotXDimension::levelChunkNeedsUpgrade(LevelChunk const& lc) const {
    return VanillaLevelChunkUpgrade::levelChunkNeedsUpgrade(lc);
}
void PlotXDimension::_upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) {
    auto isTemplate = mLevel.getLevelData().mIsFromLockedTemplate;
    return VanillaLevelChunkUpgrade::upgradeOldLimboEntity(tag, vers, isTemplate);
}

std::unique_ptr<ChunkSource>
PlotXDimension::_wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion /*ver*/) {
    return cs;
}

Vec3 PlotXDimension::translatePosAcrossDimension(Vec3 const& fromPos, DimensionType fromId) const {
    Vec3 topos;
    VanillaDimensions::convertPointBetweenDimensions(fromPos, topos, fromId, mId, mLevel.getDimensionConversionData());
    constexpr auto clampVal = 32000000.0f - 128.0f;

    topos.x = std::clamp(topos.x, -clampVal, clampVal);
    topos.z = std::clamp(topos.z, -clampVal, clampVal);

    return topos;
}

short PlotXDimension::getCloudHeight() const { return 192; }

// bool PlotXDimension::hasPrecipitationFog() const { return true; }

} // namespace plotx::world
