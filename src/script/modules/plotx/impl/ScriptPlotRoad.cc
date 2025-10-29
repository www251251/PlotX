#include "plotx/math/PlotRoad.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

namespace plotx::script::modules {


qjspp::ClassDefine const PlotXModule::ScriptPlotRoad =
    qjspp::defineClass<PlotRoad>("PlotRoad")
        .extends(ScriptPlotAABB)
        .constructor<>()
        .constructor<int, int, bool>()
        .constructor<BlockPos const&>()
        .instanceProperty("x", &PlotRoad::x)
        .instanceProperty("z", &PlotRoad::z)
        .instanceProperty("isTransverse_", &PlotRoad::isTransverse_)
        .instanceProperty("valid_", &PlotRoad::valid_)
        .instanceMethod("isTransverse", &PlotRoad::isTransverse)
        .instanceMethod("isLongitudinal", &PlotRoad::isLongitudinal)
        .instanceMethod("isValid", &PlotRoad::isValid)
        .instanceMethod("toString", &PlotRoad::toString)
        .instanceMethod("removeNeighbourBorder", &PlotRoad::removeNeighbourBorder)
        .build();


}
