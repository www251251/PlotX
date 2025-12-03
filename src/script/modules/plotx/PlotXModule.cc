#include "PlotXModule.hpp"
#include "qjspp/bind/meta/ModuleDefine.hpp"

#include <qjspp/bind/builder/ModuleDefineBuilder.hpp>


namespace plotx::script::modules {


qjspp::bind::meta::ModuleDefine const& PlotXModule::getModuleDef() {
    static auto def = qjspp::bind::defineModule("@plotx")
                          .addClass(ScriptPlotHandle)
                          .addClass(ScriptPlotRegistry)
                          .addClass(ScriptPlotAABB)
                          .addClass(ScriptPlotCoord)
                          .build();
    return def;
}


} // namespace plotx::script::modules