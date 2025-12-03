#include "MinecraftModule.hpp"

#include "qjspp/bind/builder/ModuleDefineBuilder.hpp"

namespace plotx::script::modules {

qjspp::bind::meta::ModuleDefine const& MinecraftModule::getModuleDef() {
    static auto def = qjspp::bind::defineModule("@minecraft")
                          .addClass(ScriptActor)
                          .addClass(ScriptPlayer)
                          .addClass(ScriptUUID)
                          .addEnum(ScriptModalFormCancelReason)
                          .addClass(ScriptBlockPos)
                          .addClass(ScriptVec3)
                          .addEnum(ScriptCommandPermissionLevel)
                          .addEnum(ScriptCommandFlagValue)
                          .addEnum(ScriptCommandParameterOption)
                          .addClass(ScriptCommandOrigin)
                          .addEnum(ScriptCommandOriginType)
                          .addClass(ScriptCommandOutput)
                          .build();
    return def;
}

} // namespace plotx::script::modules
