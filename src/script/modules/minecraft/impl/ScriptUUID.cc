#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"


namespace plotx::script::modules {

qjspp::ClassDefine const MinecraftModule::ScriptUUID = qjspp::defineClass<mce::UUID>("UUID")
                                                      .constructor<std::string const&>()
                                                      .instanceMethod("asString", &mce::UUID::asString)
                                                      .function("canParse", &mce::UUID::canParse)
                                                      .function("fromString", &mce::UUID::fromString)
                                                      .build();


}
