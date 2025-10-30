#include "LeviLaminaModule.hpp"

#include <qjspp/Module.hpp>

namespace plotx::script::modules {

qjspp::ModuleDefine const& LeviLaminaModule::getModuleDef() {
    static qjspp::ModuleDefine def = qjspp::ModuleDefineBuilder("@levilamina")
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
                                         .build();
    return def;
}


} // namespace plotx::script::modules