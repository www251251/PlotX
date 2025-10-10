#include "LeviLaminaDef.hpp"

#include <qjspp/Module.hpp>

namespace plotx::script::modules {

qjspp::ModuleDefine const& LeviLaminaDef::getModuleDef() {
    static qjspp::ModuleDefine def = qjspp::ModuleDefineBuilder("@levilamina")
                                         .addClass(SimpleFormDef_)
                                         .addClass(CustomFormDef_)
                                         .addClass(ModalFormDef_)
                                         .addEnum(ModalFormSelectedButtonDef_)
                                         .addClass(EventBusDef_)
                                         .addClass(EventDef_)
                                         .addClass(CancellableEventDef_)
                                         .addClass(PlayerJoinEventDef_)
                                         .addEnum(EventPriorityDef_)
                                         .addClass(KeyValueDBDef_)
                                         .build();
    return def;
}


} // namespace plotx::script::modules