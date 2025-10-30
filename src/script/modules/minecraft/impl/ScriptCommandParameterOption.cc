#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandParameterOption.h>

namespace plotx::script::modules {

qjspp::EnumDefine const MinecraftModule::ScriptCommandParameterOption =
    qjspp::defineEnum<CommandParameterOption>("CommandParameterOption")
        .value("None", CommandParameterOption::None)
        .value("EnumAutocompleteExpansion", CommandParameterOption::EnumAutocompleteExpansion)
        .value("HasSemanticConstraint", CommandParameterOption::HasSemanticConstraint)
        .value("EnumAsChainedCommand", CommandParameterOption::EnumAsChainedCommand)
        .build();

}