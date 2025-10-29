#pragma once
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {


struct MinecraftModule {
    MinecraftModule() = delete;

    static qjspp::ClassDefine const ScriptPlayer;

    static qjspp::ClassDefine const ScriptUUID;

    static qjspp::EnumDefine const ScriptModalFormCancelReason;

    static qjspp::ClassDefine const ScriptBlockPos;

    static qjspp::ClassDefine const ScriptVec3;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules