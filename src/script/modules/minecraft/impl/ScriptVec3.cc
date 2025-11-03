#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/BlockPos.h"
#include "qjspp/Binding.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"


namespace plotx::script::modules {


qjspp::ClassDefine const MinecraftModule::ScriptVec3 = qjspp::defineClass<::Vec3>("Vec3")
                                                      .constructor<float, float, float>()
                                                      .instanceProperty("x", &Vec3::x)
                                                      .instanceProperty("y", &Vec3::y)
                                                      .instanceProperty("z", &Vec3::z)
                                                      .instanceMethod("isNan", &Vec3::isNan)
                                                      .instanceMethod("isNear", &Vec3::isNear)
                                                      .instanceMethod("toJsonString", &Vec3::toJsonString)
                                                      .instanceMethod("toString", &Vec3::toString)
                                                      .build();


}
