#include "ModuleDef.hpp"
#include "qjspp/Module.hpp"

#include "script/modules/levilamina/Defines.hpp"

#include "script/modules/minecraft/defs.hpp"

#include "script/modules/plotx/defs.hpp"

namespace plotx::script {


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