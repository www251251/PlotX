#pragma once
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "qjspp/bind/meta/EnumDefine.hpp"
#include "qjspp/bind/meta/ModuleDefine.hpp"

namespace plotx::script::modules {


struct MinecraftModule {
    MinecraftModule() = delete;

    static qjspp::bind::meta::ClassDefine const ScriptActor;
    static qjspp::bind::meta::ClassDefine const ScriptPlayer;

    static qjspp::bind::meta::ClassDefine const ScriptUUID;

    static qjspp::bind::meta::EnumDefine const ScriptModalFormCancelReason;

    static qjspp::bind::meta::ClassDefine const ScriptBlockPos;

    static qjspp::bind::meta::ClassDefine const ScriptVec3;

    static qjspp::bind::meta::EnumDefine const  ScriptCommandPermissionLevel;
    static qjspp::bind::meta::EnumDefine const  ScriptCommandFlagValue;
    static qjspp::bind::meta::EnumDefine const  ScriptCommandParameterOption;
    static qjspp::bind::meta::ClassDefine const ScriptCommandOrigin;
    static qjspp::bind::meta::EnumDefine const  ScriptCommandOriginType;
    static qjspp::bind::meta::ClassDefine const ScriptCommandOutput;


    static qjspp::bind::meta::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules