#include "plotx/core/PlotRegistry.hpp"

#include "script/modules/plotx/PlotXModule.hpp"
#include "script/modules/Helper.hpp"

namespace plotx::script::modules {


qjspp::ClassDefine const PlotXModule::ScriptPlotRegistry =
    qjspp::defineClass<PlotRegistry>("PlotRegistry")
        .disableConstructor()
        .instanceMethod(
            "isAdmin",
            static_cast<bool (PlotRegistry::*)(mce::UUID const&) const>(&PlotRegistry::isAdmin),
            static_cast<bool (PlotRegistry::*)(std::string const&) const>(&PlotRegistry::isAdmin)
        )
        .instanceMethod("addAdmin", &PlotRegistry::addAdmin)
        .instanceMethod("removeAdmin", &PlotRegistry::removeAdmin)
        .build();


}