#include "mc/world/level/BlockPos.h"
#include "plotx/PlotX.hpp"
#include "plotx/math/PlotAABB.hpp"

#include "qjspp/bind/meta/ClassDefine.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>


namespace plotx::script::modules {


qjspp::bind::meta::ClassDefine const PlotXModule::ScriptPlotAABB =
    qjspp::bind::defineClass<PlotAABB>("PlotAABB")
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
