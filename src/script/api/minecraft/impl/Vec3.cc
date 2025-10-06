#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/BlockPos.h"
#include "script/api/Helper.hpp"
#include "script/api/minecraft/defs.hpp"
#include "qjspp/Binding.hpp"


namespace plotx::script::api::inline minecraft {


extern qjspp::ClassDefine const Vec3Def_ = qjspp::defineClass<Vec3>("Vec3")
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
