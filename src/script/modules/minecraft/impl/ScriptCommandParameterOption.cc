#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandParameterOption.h>
#include <qjspp/bind/builder/EnumDefineBuilder.hpp>

namespace plotx::script::modules {


qjspp::bind::meta::EnumDefine const MinecraftModule::ScriptCommandParameterOption =
    qjspp::bind::defineEnum<CommandParameterOption>("CommandParameterOption")
        .value("None", CommandParameterOption::None)
        .value("EnumAutocompleteExpansion", CommandParameterOption::EnumAutocompleteExpansion)
        .value("HasSemanticConstraint", CommandParameterOption::HasSemanticConstraint)
        .value("EnumAsChainedCommand", CommandParameterOption::EnumAsChainedCommand)
        .build();

}