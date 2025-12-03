#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandOrigin.h>
#include <qjspp/bind/builder/ClassDefineBuilder.hpp>
#include <qjspp/bind/builder/EnumDefineBuilder.hpp>

namespace qjspp::bind {

template <>
struct TypeConverter<Actor> {
    static Value toJs(Actor* actor) {
        return plotx::script::newInstanceOfGameWeak(plotx::script::modules::MinecraftModule::ScriptActor, actor);
    }
    static Actor* toCpp(Value const& val) {
        return Locker::currentEngineChecked().getNativeInstanceOf<Actor>(
            val.asObject(),
            plotx::script::modules::MinecraftModule::ScriptActor
        );
    }
};

} // namespace qjspp::bind

namespace plotx::script::modules {

qjspp::bind::meta::ClassDefine const MinecraftModule::ScriptCommandOrigin =
    qjspp::bind::defineClass<CommandOrigin>("CommandOrigin")
        .disableConstructor()
        .instanceMethod("getEntity", &CommandOrigin::getEntity)
        .instanceMethod("getOriginType", &CommandOrigin::getOriginType)
        .build();

qjspp::bind::meta::EnumDefine const MinecraftModule::ScriptCommandOriginType =
    qjspp::bind::defineEnum<CommandOriginType>("CommandOriginType")
        .value("Player", CommandOriginType::Player)
        .value("CommandBlock", CommandOriginType::CommandBlock)
        .value("MinecartCommandBlock", CommandOriginType::MinecartCommandBlock)
        .value("DevConsole", CommandOriginType::DevConsole)
        .value("Test", CommandOriginType::Test)
        .value("AutomationPlayer", CommandOriginType::AutomationPlayer)
        .value("ClientAutomation", CommandOriginType::ClientAutomation)
        .value("DedicatedServer", CommandOriginType::DedicatedServer)
        .value("Entity", CommandOriginType::Entity)
        .value("Virtual", CommandOriginType::Virtual)
        .value("GameArgument", CommandOriginType::GameArgument)
        .value("EntityServer", CommandOriginType::EntityServer)
        .value("Precompiled", CommandOriginType::Precompiled)
        .value("GameDirectorEntityServer", CommandOriginType::GameDirectorEntityServer)
        .value("Scripting", CommandOriginType::Scripting)
        .value("ExecuteContext", CommandOriginType::ExecuteContext)
        .build();

} // namespace plotx::script::modules