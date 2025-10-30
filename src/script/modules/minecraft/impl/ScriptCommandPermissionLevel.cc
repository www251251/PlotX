#include "script/modules/minecraft/MinecraftModule.hpp"

#include "qjspp/Binding.hpp"

#include <mc/server/commands/CommandPermissionLevel.h>

namespace plotx::script::modules {

qjspp::EnumDefine const MinecraftModule::ScriptCommandPermissionLevel =
    qjspp::defineEnum<CommandPermissionLevel>("CommandPermissionLevel")
        .value("Any", CommandPermissionLevel::Any)
        .value("GameDirectors", CommandPermissionLevel::GameDirectors)
        .value("Admin", CommandPermissionLevel::Admin)
        .value("Host", CommandPermissionLevel::Host)
        .value("Owner", CommandPermissionLevel::Owner)
        .value("Internal", CommandPermissionLevel::Internal)
        .build();

}