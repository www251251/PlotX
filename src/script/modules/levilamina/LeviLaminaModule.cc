#include "LeviLaminaModule.hpp"

#include "qjspp/bind/builder/ModuleDefineBuilder.hpp"

namespace plotx::script::modules {

qjspp::bind::meta::ModuleDefine const& LeviLaminaModule::getModuleDef() {
    static qjspp::bind::meta::ModuleDefine def = qjspp::bind::ModuleDefineBuilder("@levilamina")
                                                     .addClass(ScriptSimpleForm)
                                                     .addClass(ScriptCustomForm)
                                                     .addClass(ScriptModalForm)
                                                     .addEnum(ScriptModalFormSelectedButton)
                                                     .addClass(ScriptEventBus)
                                                     .addClass(ScriptEvent)
                                                     .addClass(ScriptCancellableEvent)
                                                     .addClass(ScriptPlayerJoinEvent)
                                                     .addEnum(ScriptEventPriority)
                                                     .addClass(ScriptKeyValueDB)
                                                     .addClass(ScriptLogger)
                                                     .addEnum(ScriptLogLevel)
                                                     .addClass(ScriptCommandRegistrar)
                                                     .addClass(ScriptCommandHandle)
                                                     .addClass(ScriptRuntimeOverload)
                                                     .addEnum(ScriptCommandParamKind)
                                                     .addClass(ScriptPlayerInfo)
                                                     .addClass(ScriptI18n)
                                                     .build();
    return def;
}


} // namespace plotx::script::modules