#include "MinecraftModule.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {

qjspp::ModuleDefine const& MinecraftModule::getModuleDef() {
    static auto def = qjspp::defineModule("@minecraft")
                          .addClass(ScriptPlayer)
                          .addClass(ScriptUUID)
                          .addEnum(ScriptModalFormCancelReason)
                          .addClass(ScriptBlockPos)
                          .addClass(ScriptVec3)
                          .build();
    return def;
}

} // namespace plotx::script::modules
