#include "ModuleDef.hpp"
#include "qjspp/Module.hpp"

#include "script/api/levilamina/defs.hpp"

#include "script/api/minecraft/defs.hpp"

#include "script/api/plotx/defs.hpp"

namespace plotx::script {


qjspp::ModuleDefine const& GetLeviLaminaModuleDef() {
    static auto def = qjspp::defineModule("levilamina")
                          .exportClass(api::SimpleFormDef_)
                          .exportClass(api::CustomFormDef_)
                          .exportClass(api::ModalFormDef_)
                          .exportEnum(api::ModalFormSelectedButtonDef_)
                          .exportClass(api::EventBusDef_)
                          .exportClass(api::EventDef_)
                          .exportClass(api::CancellableEventDef_)
                          .exportClass(api::PlayerJoinEventDef_)
                          .exportEnum(api::EventPriorityDef_)
                          .exportClass(api::KeyValueDBDef_)
                          .build();
    return def;
}

qjspp::ModuleDefine const& GetMinecraftModuleDef() {
    static auto def = qjspp::defineModule("minecraft")
                          .exportClass(api::PlayerDef_)
                          .exportClass(api::UUIDDef_)
                          .exportEnum(api::ModalFormCancelReasonDef_)
                          .exportClass(api::BlockPosDef_)
                          .exportClass(api::Vec3Def_)
                          .build();
    return def;
}

qjspp::ModuleDefine const& GetPlotXModuleDef() {
    static auto def = qjspp::defineModule("plotx").exportClass(api::LoggerDef_).exportClass(api::PlotAABBDef_).build();
    return def;
}


} // namespace plotx::script