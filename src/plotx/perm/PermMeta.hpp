#pragma once
#include <string>

namespace plotx {

enum class PermCategory {
    Core,       // 核心
    Combat,     // 战斗
    Item,       // 物品
    Entity,     // 实体
    Tools,      // 工具
    Functional, // 功能方块
    Decorative, // 装饰
    Special,    // 特殊
    Environment // 环境
};

struct PermMeta {
    bool         defaultValue{false};          // 默认值
    PermCategory category{PermCategory::Core}; // 分类
    std::string  name{};                       // 原始字符串
};

} // namespace plotx