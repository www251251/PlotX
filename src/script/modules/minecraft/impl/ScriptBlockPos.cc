#include "mc/world/level/BlockPos.h"
#include "qjspp/Binding.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"


namespace plotx::script::modules {

qjspp::ClassDefine const MinecraftModule::ScriptBlockPos = qjspp::defineClass<::BlockPos>("BlockPos")
                                                          .constructor<int, int, int>()
                                                          .instanceProperty("x", &BlockPos::x)
                                                          .instanceProperty("y", &BlockPos::y)
                                                          .instanceProperty("z", &BlockPos::z)
                                                          .build();

}
