#include "mc/world/level/BlockPos.h"
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>


namespace plotx::script::modules {

qjspp::bind::meta::ClassDefine const MinecraftModule::ScriptBlockPos = qjspp::bind::defineClass<::BlockPos>("BlockPos")
                                                                           .constructor<int, int, int>()
                                                                           .instanceProperty("x", &BlockPos::x)
                                                                           .instanceProperty("y", &BlockPos::y)
                                                                           .instanceProperty("z", &BlockPos::z)
                                                                           .build();

}
