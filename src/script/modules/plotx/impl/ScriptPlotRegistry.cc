#include "plotx/core/PlotRegistry.hpp"

#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

namespace plotx::script::modules {


qjspp::ClassDefine const PlotXModule::ScriptPlotRegistry =
    qjspp::defineClass<PlotRegistry>("PlotRegistry")
        .disableConstructor()
        .instanceMethod("isAdmin", &PlotRegistry::isAdmin)
        .instanceMethod("addAdmin", &PlotRegistry::addAdmin)
        .instanceMethod("removeAdmin", &PlotRegistry::removeAdmin)
        .build();


}