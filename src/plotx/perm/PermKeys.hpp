#pragma once
#include "plotx/infra/HashedStringView.hpp"

namespace plotx::perm_keys {

// === 基础权限 ===
constexpr auto allowDestroy  = "allowDestroy"_hsv;  // 破坏方块
constexpr auto allowPlace    = "allowPlace"_hsv;    // 放置方块
constexpr auto allowInteract = "allowInteract"_hsv; // 基础交互(开箱子、使用按钮等)

// === 战斗权限 ===
constexpr auto allowPlayerDamage        = "allowPlayerDamage"_hsv;        // 玩家PVP
constexpr auto allowMonsterDamage       = "allowMonsterDamage"_hsv;       // 攻击敌对生物
constexpr auto allowPassiveDamage       = "allowPassiveDamage"_hsv;       // 攻击友好生物
constexpr auto allowSpecialDamage       = "allowSpecialDamage"_hsv;       // 攻击特殊实体(船、矿车、画等)
constexpr auto allowCustomSpecialDamage = "allowCustomSpecialDamage"_hsv; // 攻击自定义特殊实体

// === 物品权限 ===
constexpr auto allowPickupItem       = "allowPickupItem"_hsv;       // 拾取物品
constexpr auto allowDropItem         = "allowDropItem"_hsv;         // 丢弃物品
constexpr auto allowProjectileCreate = "allowProjectileCreate"_hsv; // 创建弹射物

// === 实体权限 ===
constexpr auto allowRideEntity     = "allowRideEntity"_hsv;     // 骑乘实体
constexpr auto allowInteractEntity = "allowInteractEntity"_hsv; // 实体交互
constexpr auto allowActorDestroy   = "allowActorDestroy"_hsv;   // 实体破坏

// === 工具权限 ===
constexpr auto useTools               = "useTools"_hsv;               // 使用工具(斧头去皮、锄头、铲子、骨粉等)
constexpr auto useBucket              = "useBucket"_hsv;              // 使用桶
constexpr auto allowFishingRodAndHook = "allowFishingRodAndHook"_hsv; // 使用钓鱼竿

// === 功能方块权限 ===
constexpr auto useFurnaces       = "useFurnaces"_hsv;       // 使用熔炉类(熔炉、高炉、烟熏炉)
constexpr auto useCraftingBlocks = "useCraftingBlocks"_hsv; // 使用工作台类(工作台、制图台、锻造台、砂轮、附魔台等)
constexpr auto useStorageBlocks  = "useStorageBlocks"_hsv;  // 使用存储类(箱子、木桶、漏斗、潜影盒等)
constexpr auto useRedstoneBlocks = "useRedstoneBlocks"_hsv; // 使用红石类(比较器、中继器、发射器、投掷器等)
constexpr auto useUtilityBlocks  = "useUtilityBlocks"_hsv;  // 使用功能类(信标、讲台、炼药锅、重生锚等)

// === 装饰交互权限 ===
constexpr auto useDecorative = "useDecorative"_hsv; // 使用装饰类(音符盒、唱片机、物品展示框、盔甲架、花盆等)
constexpr auto useMovement   = "useMovement"_hsv;   // 使用移动类(门、活板门、栅栏门、压力板等)
constexpr auto editSigns     = "editSigns"_hsv;     // 编辑告示牌
constexpr auto editFlowerPot = "editFlowerPot"_hsv; // 编辑花盆

// === 特殊权限 ===
constexpr auto useBed               = "useBed"_hsv;               // 使用床
constexpr auto useCake              = "useCake"_hsv;              // 吃蛋糕
constexpr auto placeVehicles        = "placeVehicles"_hsv;        // 放置载具(船、矿车)
constexpr auto allowAttackDragonEgg = "allowAttackDragonEgg"_hsv; // 点击龙蛋

// === 环境权限 ===
constexpr auto allowFireSpread           = "allowFireSpread"_hsv;           // 火焰蔓延
constexpr auto allowFarmDecay            = "allowFarmDecay"_hsv;            // 耕地退化
constexpr auto allowPistonPushOnBoundary = "allowPistonPushOnBoundary"_hsv; // 活塞推动
constexpr auto allowRedstoneUpdate       = "allowRedstoneUpdate"_hsv;       // 红石更新
constexpr auto allowExplode              = "allowExplode"_hsv;              // 爆炸
constexpr auto allowBlockFall            = "allowBlockFall"_hsv;            // 方块掉落
constexpr auto allowWitherDestroy        = "allowWitherDestroy"_hsv;        // 凋零破坏
constexpr auto allowLiquidFlow           = "allowLiquidFlow"_hsv;           // 液体流动
constexpr auto allowSculkBlockGrowth     = "allowSculkBlockGrowth"_hsv;     // 幽匿尖啸体生长
constexpr auto allowMonsterSpawn         = "allowMonsterSpawn"_hsv;         // 怪物生成
constexpr auto allowAnimalSpawn          = "allowAnimalSpawn"_hsv;          // 动物生成
constexpr auto allowEndermanLeaveBlock   = "allowEndermanLeaveBlock"_hsv;   // 末影人放下方块


} // namespace plotx::perm_keys