#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"


#include <mc/world/actor/Actor.h>
#include <qjspp/bind/builder/ClassDefineBuilder.hpp>

namespace plotx::script::modules {


qjspp::bind::meta::ClassDefine const MinecraftModule::ScriptActor =
    qjspp::bind::defineClass<Actor>("Actor")
        .disableConstructor()
        .instanceMethod("getTypeName", &Actor::getTypeName)
        .instanceMethod("getFeetPos", &Actor::getFeetPos)
        .instanceMethod("getFeetBlockPos", &Actor::getFeetBlockPos)
        .instanceMethod("isSimulatedPlayer", &Actor::isSimulatedPlayer)
        .instanceMethod(
            "teleport",
            [](void* inst, qjspp::Arguments const& args) -> qjspp::Value {
                auto actor = static_cast<Actor*>(inst);
                if (args.length() < 2) {
                    throw qjspp::JsException{"too few arguments"};
                }
                if (!args[0].isObject() || !args[1].isNumber()) {
                    throw qjspp::JsException{"wrong argument type"};
                }
                auto pos = args.engine()->getNativeInstanceOf<Vec3>(args[0].asObject(), MinecraftModule::ScriptVec3);
                if (pos) {
                    actor->teleport(*pos, args[1].asNumber().getInt32());
                }
                return {};
            }
        )
        .instanceMethod("despawn", &Actor::despawn)
        .instanceMethod("kill", &Actor::kill)
        .instanceMethod(
            "getDimensionId",
            [](void* inst, qjspp::Arguments const&) -> qjspp::Value {
                return qjspp::Number{static_cast<Actor*>(inst)->getDimensionId()};
            }
        )
        .instanceMethod("getPosition", &Actor::getPosition)
        .instanceMethod("isPlayer", &Actor::isPlayer)
        .build();


} // namespace plotx::script::modules