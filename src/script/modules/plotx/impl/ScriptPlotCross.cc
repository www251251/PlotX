#include "plotx/math/PlotCross.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"


namespace plotx::script::modules {


qjspp::ClassDefine const PlotXModule::ScriptPlotCross = qjspp::defineClass<PlotCross>("PlotCross")
                                                       .extends(ScriptPlotAABB)
                                                       .constructor<>()
                                                       .constructor<int, int>()
                                                       .constructor<BlockPos const&>()
                                                       .instanceProperty("x", &PlotCross::x)
                                                       .instanceProperty("z", &PlotCross::z)
                                                       .instanceProperty("valid_", &PlotCross::valid_)
                                                       .instanceMethod("toString", &PlotCross::toString)
                                                       .instanceMethod("isValid", &PlotCross::isValid)
                                                       .build();

}