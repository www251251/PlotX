#pragma once
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"

#include "plotx/Global.hpp"
#include "plotx/world/Helper.hpp"

namespace plotx::world {

class PlotGenerator final : public FlatWorldGenerator {
public:
    struct ThreadData {
        helper::ArrayBuffer buffer_;
        BlockVolume         volume_;

        explicit ThreadData(PlotGenerator& generator);
        ~ThreadData() = default;
        PLOTX_DISALLOW_COPY_AND_MOVE(ThreadData);
    };


    PlotGenerator(Dimension& dimension, uint seed, Json::Value const& options);
    void loadChunk(LevelChunk& lc, bool forceImmediateReplacementDataLoad) override;

private:
    Block const* airBlock_{nullptr};     // 空气
    Block const* bedrockBlock_{nullptr}; // 基岩
    Block const* surfaceBlock_{nullptr}; // 地表方块
    Block const* fillBlock_{nullptr};    // 填充方块
    Block const* borderBlock_{nullptr};  // 边框
    Block const* roadBlock_{nullptr};    // 路面
    int const    generatorHeight_{0};    // 生成高度
    int const    borderHeight_{0};       // 边框高度

    friend struct ThreadData;
};

} // namespace plotx::world
