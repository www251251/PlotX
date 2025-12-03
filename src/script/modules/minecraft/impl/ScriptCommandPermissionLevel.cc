#include "script/modules/minecraft/MinecraftModule.hpp"

#include "qjspp/bind/meta/ClassDefine.hpp"

#include <mc/server/commands/CommandPermissionLevel.h>
#include <qjspp/bind/builder/EnumDefineBuilder.hpp>

namespace plotx::script::modules {

qjspp::bind::meta::EnumDefine const MinecraftModule::ScriptCommandPermissionLevel =
    qjspp::bind::defineEnum<CommandPermissionLevel>("CommandPermissionLevel")
        .value("Any", CommandPermissionLevel::Any)
        .value("GameDirectors", CommandPermissionLevel::GameDirectors)
        .value("Admin", CommandPermissionLevel::Admin)
        .value("Host", CommandPermissionLevel::Host)
        .value("Owner", CommandPermissionLevel::Owner)
        .value("Internal", CommandPermissionLevel::Internal)
        .build();

}