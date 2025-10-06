#include "mc/world/level/BlockPos.h"
#include "qjspp/Binding.hpp"
#include "script/api/Helper.hpp"
#include "script/api/minecraft/defs.hpp"


namespace plotx::script::api::inline minecraft {

qjspp::ClassDefine const BlockPosDef_ = qjspp::defineClass<BlockPos>("BlockPos")
                                            .constructor<int, int, int>()
                                            .instanceProperty("x", &BlockPos::x)
                                            .instanceProperty("y", &BlockPos::y)
                                            .instanceProperty("z", &BlockPos::z)
                                            .build();

}
