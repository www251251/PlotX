#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandFlag.h>

namespace plotx::script::modules {

qjspp::EnumDefine const MinecraftModule::ScriptCommandFlagValue =
    qjspp::defineEnum<CommandFlagValue>("CommandFlagValue")
        .value("None", CommandFlagValue::None)
        .value("UsageTest", CommandFlagValue::UsageTest)
        .value("HiddenFromCommandBlockOrigin", CommandFlagValue::HiddenFromCommandBlockOrigin)
        .value("HiddenFromPlayerOrigin", CommandFlagValue::HiddenFromPlayerOrigin)
        .value("HiddenFromAutomationOrigin", CommandFlagValue::HiddenFromAutomationOrigin)
        .value("SyncLocal", CommandFlagValue::SyncLocal)
        .value("ExecuteDisallowed", CommandFlagValue::ExecuteDisallowed)
        .value("TypeMessage", CommandFlagValue::TypeMessage)
        .value("NotCheat", CommandFlagValue::NotCheat)
        .value("Async", CommandFlagValue::Async)
        .value("NoEditor", CommandFlagValue::NoEditor)
        .value("Hidden", CommandFlagValue::Hidden)
        .value("Removed", CommandFlagValue::Removed)
        .build();

}