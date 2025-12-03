#include "plotx/core/PlotRegistry.hpp"

#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

#include "qjspp/bind/builder/ClassDefineBuilder.hpp"

namespace plotx::script::modules {


qjspp::bind::meta::ClassDefine const PlotXModule::ScriptPlotRegistry =
    qjspp::bind::defineClass<PlotRegistry>("PlotRegistry")
        .disableConstructor()
        .instanceMethod("isAdmin", &PlotRegistry::isAdmin)
        .instanceMethod("addAdmin", &PlotRegistry::addAdmin)
        .instanceMethod("removeAdmin", &PlotRegistry::removeAdmin)
        .build();


}