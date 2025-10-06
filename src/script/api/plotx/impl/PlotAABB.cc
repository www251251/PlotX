#include "plotx/math/PlotAABB.hpp"
#include "mc/world/level/BlockPos.h"
#include "plotx/PlotX.hpp"
#include "script/api/Helper.hpp"
#include "script/api/plotx/defs.hpp"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/JsException.hpp"
#include "qjspp/Values.hpp"


namespace plotx::script::api::inline plotx {


qjspp::ClassDefine const PlotAABBDef_ =
    qjspp::defineClass<PlotAABB>("PlotAABB")
        .constructor<BlockPos const&, BlockPos const&>() // constructor(min, max)
        .instanceProperty(
            "min",
            [](void* inst, qjspp::Arguments const& args) -> qjspp::Value {
                return args.engine()->newInstanceOfView(
                    BlockPosDef_,
                    static_cast<void*>(&static_cast<PlotAABB*>(inst)->min),
                    args.thiz() // Associated Lifecycle
                );
            },
            [](void* inst, qjspp::Arguments const& args) -> qjspp::Value {
                if (!args[0].isObject() || !args.engine()->isInstanceOf(args[0].asObject(), BlockPosDef_)) {
                    throw qjspp::JsException{qjspp::JsException::Type::TypeError, "Expected object of type BlockPos"};
                }
                static_cast<PlotAABB*>(inst)->min =
                    *args.engine()->getNativeInstanceOf<BlockPos>(args[0].asObject(), BlockPosDef_);
                return {}; // undefined
            }
        )
        .instanceProperty(
            "max",
            [](void* inst, qjspp::Arguments const& args) -> qjspp::Value {
                return args.engine()->newInstanceOfView(
                    BlockPosDef_,
                    static_cast<void*>(&static_cast<PlotAABB*>(inst)->max),
                    args.thiz() // Associated Lifecycle
                );
            },
            [](void* inst, qjspp::Arguments const& args) -> qjspp::Value {
                if (!args[0].isObject() || !args.engine()->isInstanceOf(args[0].asObject(), BlockPosDef_)) {
                    throw qjspp::JsException{qjspp::JsException::Type::TypeError, "Expected object of type BlockPos"};
                }
                static_cast<PlotAABB*>(inst)->max =
                    *args.engine()->getNativeInstanceOf<BlockPos>(args[0].asObject(), BlockPosDef_);
                return {}; // undefined
            }
        )
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


} // namespace plotx::script::api::inline plotx
