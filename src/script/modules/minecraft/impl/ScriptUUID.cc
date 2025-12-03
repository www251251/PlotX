#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>


namespace plotx::script::modules {

qjspp::bind::meta::ClassDefine const MinecraftModule::ScriptUUID = qjspp::bind::defineClass<mce::UUID>("UUID")
                                                                       .constructor<std::string const&>()
                                                                       .instanceMethod("asString", &mce::UUID::asString)
                                                                       .function("canParse", &mce::UUID::canParse)
                                                                       .function("fromString", &mce::UUID::fromString)
                                                                       .build();


}
