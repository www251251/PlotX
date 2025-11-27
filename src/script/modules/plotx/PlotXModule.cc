#include "PlotXModule.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {


qjspp::ModuleDefine const& PlotXModule::getModuleDef() {
    static auto def = qjspp::defineModule("@plotx")
                          // .addClass(ScriptPlotHandle) // TODO: impl
                          .addClass(ScriptPlotRegistry)
                          .addClass(ScriptPlotAABB)
                          .addClass(ScriptPlotCoord)
                          .build();
    return def;
}


} // namespace plotx::script::modules