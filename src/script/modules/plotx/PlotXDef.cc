#include "PlotXDef.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {


qjspp::ModuleDefine const& PlotXDef::getModuleDef() {
    static auto def = qjspp::defineModule("@plotx").addClass(LoggerDef_).addClass(PlotAABBDef_).build();
    return def;
}


} // namespace plotx::script::modules