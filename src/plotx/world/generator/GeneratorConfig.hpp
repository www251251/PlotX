#pragma once
#include <string>

namespace plotx::world {


struct GeneratorConfig {
    int plotWidth{128};     // 地皮大小(长宽)
    int roadWidth{5};       // 道路宽度
    int borderWidth{1};     // 地皮边框宽度
    int generatorHeight{0}; // 地皮生成高度 -64 ~ 320

    std::string surfaceBlock = "minecraft:grass";            // 地皮表面方块
    std::string fillBlock    = "minecraft:dirt";             // 地皮填充方块
    std::string roadBlock    = "minecraft:birch_planks";     // 道路方块
    std::string borderBlock  = "minecraft:stone_block_slab"; // 地皮边框方块
    std::string biome        = "minecraft:plains";           // 地皮世界生物群系
};


} // namespace plotx::world