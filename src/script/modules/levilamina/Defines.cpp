#include "Defines.hpp"

#include <qjspp/Module.hpp>

namespace plotx::script::api::levilamina {

qjspp::ModuleDefine const& Defines::getModuleDef() {
    static qjspp::ModuleDefine def = qjspp::ModuleDefineBuilder("@levilamina")
                                         .exportClass(SimpleForm)
                                         .exportClass(CustomForm)
                                         .exportClass(ModalForm)
                                         .exportEnum(ModalFormSelectedButton)
                                         .exportClass(EventBus)
                                         .exportClass(Event)
                                         .exportClass(CancellableEventDef_)
                                         .exportClass(PlayerJoinEvent)
                                         .exportEnum(EventPriority)
                                         .exportClass(KeyValueDB)
                                         .build();
    return def;
}


} // namespace plotx::script::api::levilamina