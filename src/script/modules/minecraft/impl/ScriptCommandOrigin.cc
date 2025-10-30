#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandOrigin.h>

namespace plotx::script::modules {

// TODO: im
qjspp::ClassDefine const MinecraftModule::ScriptCommandOrigin = qjspp::defineClass<CommandOrigin>("CommandOrigin")
                                                                    .disableConstructor()
                                                                    .build();

}