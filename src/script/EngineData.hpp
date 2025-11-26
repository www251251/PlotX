#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include <ll/api/i18n/I18n.h>

namespace plotx::script {

class ScriptMod;

using EngineID = uint64_t;

struct EngineData {
    EngineID                        id_;
    std::weak_ptr<ScriptMod>        mod_;
    std::string                     modName_;
    std::unique_ptr<ll::i18n::I18n> i18n_;
};

} // namespace plotx::script