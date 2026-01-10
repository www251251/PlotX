# PlotX — 地皮系统

基于 LeviLamina 开发的地皮系统（前生为 PlotCraft）

## 功能

- 地皮生成 / 管理 / 保护 / 交易 / 查找 / 评论
- 记不清了，自己看吧

## 安装

本项目提供了两个 flavor(风味), 一个是基于原版替换地形生成器，另一个是基于多维度注册独立维度

> 注意:  
> 两个 flavor 不能同时安装，否则会导致冲突

### vanilla flavor

本 flavor 通过 Hook 修改了原版维度的地形生成器创建，替换为 PlotX 的地皮生成器

> 注意：由于是替换生成器，请再全新的地图上使用，否则会导致地形不连贯

目前仅支持替换 Overworld(主世界) 维度，其它维度暂不支持修改

文件名：`PlotX-windows-x64-vanilla.zip`

### mdim flavor

本 flavor 通过注册独立维度，不影响原版的维度，但需要额外安装 `MoreDimensions` 插件

文件名：`PlotX-windows-x64-mdim.zip`

## 命令

```log
? plotx
16:08:46.692 INFO [Server] plotx:
16:08:46.692 INFO [Server] PlotX - 地皮系统
16:08:46.692 INFO [Server] Usage:
16:08:46.692 INFO [Server] - /plotx
16:08:46.692 INFO [Server] - /plotx admin <add|remove> <name: string>
16:08:46.692 INFO [Server] - /plotx admins <add|remove> <players: target>
16:08:46.692 INFO [Server] - /plotx current
16:08:46.692 INFO [Server] - /plotx switch_dimension
16:08:46.692 INFO [Server] - /plotx teleport unowned_plot
```

### `/plotx`

打开 PlotX 主菜单

### `/plotx admin <add|remove> <name: string>`

添加或移除管理员

> 管理员需要进入过一次服务器，否则无法查询到玩家信息

### `/plotx admins <add|remove> <players: target>`

添加或移除管理员

> 此重载需要目标玩家在线，否则无法添加  
> 每次仅支持操作一个玩家，不支持选择多个玩家

### `/plotx current`

管理当前所在位置的地皮(如果无主则显示购买界面)

### `/plotx switch_dimension`

切换维度

> 行为：
>
> - 如果当前维度为地皮维度，则回到玩家的重生点
> - 如果当前维度不是地皮维度，则进入地皮维度

### `/plotx teleport unowned_plot`

查询并传送到距离 (0,0) 点最近的无主地皮

## 配置文件

```json
{
  "version": 2,
  "generator": {
    // 请谨慎修改以下配置
    // 当确认无语后再进入地皮维度，否则会造成前后的地形不一致、不连贯
    "plotWidth": 128, // 地皮宽度 (128x128)
    "roadWidth": 5, // 道路宽度
    "borderWidth": 1, // 边界宽度
    "generatorHeight": 0, // 生成高度 (此项控制世界高度，范围 -64 ~ 319)
    "surfaceBlock": "minecraft:grass", // 地皮表面方块
    "fillBlock": "minecraft:dirt", // 地皮世界填充方块
    "roadBlock": "minecraft:birch_planks", // 道路方块
    "borderBlock": "minecraft:stone_block_slab", // 边界方块
    "biome": "minecraft:plains" // 地皮生物群系
  },
  "economy": {
    "type": "Null", // 经济系统类型，可用值：Null 使用经济系统 / LegacyMoney / Scoreboard
    "scoreboard": "money" // 如果使用 Scoreboard 经济系统，则指定计分板名称
  },
  "plot": {
    "sellPrice": 100, // 地皮出售价格
    "telemetry": true // 是否启用遥测
  },
  "interceptor": {
    // 拦截器配置 (默认为全部拦截)
    // 如果关闭某个监听器，则会导致一些权限失效
    "listeners_": {
      "PlayerDestroyBlockEvent": true, // 玩家破坏方块事件
      "PlayerPlacingBlockEvent": true, // 玩家放置方块事件
      "PlayerInteractBlockEvent": true, // 玩家与方块交互事件
      "PlayerAttackEvent": true, // 玩家攻击事件
      "PlayerPickUpItemEvent": true, // 玩家拾取物品事件
      "SpawnedMobEvent": true, // 生成生物事件
      "ActorHurtEvent": true, // 生物受伤事件
      "FireSpreadEvent": true, // 火焰蔓延事件
      "ActorDestroyBlockEvent": true, // 生物破坏方块事件
      "MobTakeBlockBeforeEvent": true, // 生物破坏方块事件
      "MobPlaceBlockBeforeEvent": true, // 生物放置方块事件
      "ActorRideBeforeEvent": true, // 生物骑乘事件
      "MobHurtEffectBeforeEvent": true, // 生物受伤效果事件
      "ActorTriggerPressurePlateBeforeEvent": true, // 生物触发压力板事件
      "ProjectileCreateBeforeEvent": true, // 投掷物生成事件
      "PlayerInteractEntityBeforeEvent": true, // 玩家与实体交互事件
      "ArmorStandSwapItemBeforeEvent": true, // 盔甲架交换物品事件
      "PlayerDropItemBeforeEvent": true, // 玩家丢弃物品事件
      "PlayerOperatedItemFrameBeforeEvent": true, // 玩家操作物品展示框事件
      "PlayerEditSignBeforeEvent": true, // 玩家编辑告示牌事件
      "ExplosionBeforeEvent": true, // 爆炸事件
      "FarmDecayBeforeEvent": true, // 农田枯萎事件
      "PistonPushBeforeEvent": true, // 活塞推动事件
      "RedstoneUpdateBeforeEvent": true, // 红石更新事件
      "BlockFallBeforeEvent": true, // 方块掉落事件
      "WitherDestroyBeforeEvent": true, // 凋落破坏事件
      "MossGrowthBeforeEvent": true, // 苔藓生长事件
      "LiquidFlowBeforeEvent": true, // 液体流动事件
      "DragonEggBlockTeleportBeforeEvent": true, // 龙蛋方块传送事件
      "SculkBlockGrowthBeforeEvent": true, // 幽匿生长事件
      "SculkSpreadBeforeEvent": true // 幽匿蔓延事件
    },
    "hooks_": {}
  }
}
```

## 贡献

欢迎提交 Pull Request 或 Issue

### 许可 LICENSE

本项目遵循 AGPL-3.0 开源协议
