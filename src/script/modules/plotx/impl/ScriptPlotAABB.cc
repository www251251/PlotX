#include "plotx/math/PlotAABB.hpp"
#include "mc/world/level/BlockPos.h"
#include "plotx/PlotX.hpp"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/JsException.hpp"
#include "qjspp/Values.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"
#include "script/modules/plotx/PlotXModule.hpp"


namespace plotx::script::modules {


qjspp::ClassDefine const PlotXModule::ScriptPlotAABB =
    qjspp::defineClass<PlotAABB>("PlotAABB")
        .constructor<>()
        .constructor<BlockPos const&, BlockPos const&>()
        .instancePropertyRef("min", &PlotAABB::min, MinecraftModule::ScriptBlockPos)
        .instancePropertyRef("max", &PlotAABB::max, MinecraftModule::ScriptBlockPos)
        .instanceMethod("toString", &PlotAABB::toString)
        .instanceMethod("getMin", static_cast<BlockPos& (PlotAABB::*)()>(&PlotAABB::getMin))
        .instanceMethod("getMax", static_cast<BlockPos& (PlotAABB::*)()>(&PlotAABB::getMax))
        .instanceMethod("isOnEdge", &PlotAABB::isOnEdge)
        .instanceMethod("contains", &PlotAABB::contains)
        .instanceMethod("getEdges", &PlotAABB::getEdges)
        .instanceMethod("getVertices", &PlotAABB::getVertices)
        .instanceMethod("forEach", &PlotAABB::forEach)
        .instanceMethod("forEachLayer", &PlotAABB::forEachLayer)
        .instanceMethod("forEachEdge", &PlotAABB::forEachEdge)
        .instanceMethod("forEachEdgeLayer", &PlotAABB::forEachEdgeLayer)
        // .instanceMethod("fillLayer", &PlotAABB::fillLayer) // TODO: Block binding
        // .instanceMethod("fillEdgeLayer", &PlotAABB::fillEdgeLayer)
        .build();


} // namespace plotx::script::modules
