#include "plotx/core/PlotRegistry.hpp"

#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

#include "qjspp/bind/builder/ClassDefineBuilder.hpp"


namespace qjspp::bind {

template <>
struct TypeConverter<std::shared_ptr<plotx::PlotHandle>> {
    static Value toJs(std::shared_ptr<plotx::PlotHandle> sptr) {
        return sptr ? Null{}
                    : Locker::currentEngineChecked()
                          .newInstanceOfShared(plotx::script::modules::PlotXModule::ScriptPlotHandle, std::move(sptr))
                          .asValue();
    }
    static plotx::PlotHandle* toCpp(Value const&) {
        throw JsException{JsException::Type::InternalError, "Cannot convert JS value to PlotHandle"};
    }
};

} // namespace qjspp::bind


namespace plotx::script::modules {


qjspp::bind::meta::ClassDefine const PlotXModule::ScriptPlotRegistry =
    qjspp::bind::defineClass<PlotRegistry>("PlotRegistry")
        .disableConstructor()
        .instanceMethod("isAdmin", &PlotRegistry::isAdmin)
        .instanceMethod("addAdmin", &PlotRegistry::addAdmin)
        .instanceMethod("removeAdmin", &PlotRegistry::removeAdmin)
        .instanceMethod(
            "hasPlot",
            static_cast<bool (PlotRegistry::*)(int, int) const>(&PlotRegistry::hasPlot),
            static_cast<bool (PlotRegistry::*)(PlotCoord const&) const>(&PlotRegistry::hasPlot)
        )
        .instanceMethod(
            "getPlot",
            static_cast<std::shared_ptr<PlotHandle> (PlotRegistry::*)(int, int) const>(&PlotRegistry::getPlot),
            static_cast<std::shared_ptr<PlotHandle> (PlotRegistry::*)(PlotCoord const&) const>(&PlotRegistry::getPlot)
        )
        .instanceMethod(
            "removePlot",
            static_cast<bool (PlotRegistry::*)(int, int)>(&PlotRegistry::removePlot),
            static_cast<bool (PlotRegistry::*)(PlotCoord const&)>(&PlotRegistry::removePlot)
        )
        .instanceMethod(
            "newPlot",
            static_cast<std::shared_ptr<PlotHandle> (PlotRegistry::*)(PlotCoord const&, mce::UUID const&)>(
                &PlotRegistry::newPlot
            ),
            static_cast<std::shared_ptr<PlotHandle> (PlotRegistry::*)(int, int, mce::UUID const&)>(
                &PlotRegistry::newPlot
            )
        )
        .build();


}