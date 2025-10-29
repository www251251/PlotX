#include "plotx/math/PlotCoord.hpp"


#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

namespace plotx::script::modules {

qjspp::ClassDefine const PlotXModule::ScriptPlotCoord =
    qjspp::defineClass<PlotCoord>("PlotCoord")
        .extends(ScriptPlotAABB)
        .constructor<int, int>()
        .constructor<BlockPos const&>()
        .instanceProperty("x", &PlotCoord::x)
        .instanceProperty("z", &PlotCoord::z)
        .instanceProperty("valid_", &PlotCoord::valid_)
        .instanceMethod("toString", &PlotCoord::toString)
        .instanceMethod("isValid", &PlotCoord::isValid)
        .instanceMethod("tryFixBorder", &PlotCoord::tryFixBorder)
        .instanceMethod("removeBorder", &PlotCoord::removeBorder)
        .instanceMethod("removeBorderCorners", &PlotCoord::removeBorderCorners)
        .build();

} // namespace plotx::script::modules