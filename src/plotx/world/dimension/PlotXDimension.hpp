#pragma once
#include "mc/world/level/dimension/Dimension.h"

#include "more_dimensions/api/dimension/CustomDimensionManager.h"

namespace plotx::world {

class PlotXDimension : public Dimension {
public:
    PlotXDimension(std::string const& name, more_dimensions::DimensionFactoryInfo const& info);

    static CompoundTag generateNewData();

    std::unique_ptr<WorldGenerator> createGenerator(br::worldgen::StructureSetRegistry const&) override;

    void upgradeLevelChunk(ChunkSource& chunkSource, LevelChunk& oldLc, LevelChunk& newLc) override;

    void fixWallChunk(ChunkSource& cs, LevelChunk& lc) override;

    bool levelChunkNeedsUpgrade(LevelChunk const& lc) const override;

    void _upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) override;

    std::unique_ptr<ChunkSource>
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion ver) override;

    Vec3 translatePosAcrossDimension(Vec3 const& pos, DimensionType did) const override;

    short getCloudHeight() const override;

    // bool hasPrecipitationFog() const override;
};

} // namespace plotx::world
