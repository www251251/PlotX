#include "plotx/math/PlotCoord.hpp"


#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>

namespace plotx::script::modules {

qjspp::bind::meta::ClassDefine const PlotXModule::ScriptPlotCoord =
    qjspp::bind::defineClass<PlotCoord>("PlotCoord")
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