#pragma once
#include <string>

#include "plotx/world/generator/GeneratorConfig.hpp"

#include "perm_core/interceptor/InterceptorConfig.hpp"


namespace plotx {

struct Config {
    int version{2};

    world::GeneratorConfig generator;

    struct EconomyConfig {
        enum class Type { Null, LegacyMoney, Scoreboard };
        Type        type{Type::Null};
        std::string scoreboard = "money";
    } economy;

    struct {
        int sellPrice{100}; // 地皮出售价格
    } plot;

    permc::InterceptorConfig interceptor;
};

inline Config gConfig_;

} // namespace plotx