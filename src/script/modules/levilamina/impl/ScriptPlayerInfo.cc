#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <ll/api/service/PlayerInfo.h>

#include "qjspp/bind/TypeConverter.hpp"
#include "qjspp/bind/builder/ClassDefineBuilder.hpp"
#include "qjspp/bind/meta/ClassDefine.hpp"

namespace plotx::script::modules {

using PlayerInfo = ll::service::PlayerInfo;
using ReturnType = optional_ref<PlayerInfo::PlayerInfoEntry const>;

qjspp::Value PlayerInfo_CopyValue(ReturnType const& p) {
    if (!p.has_value()) {
        return qjspp::Null{};
    }
    auto result = qjspp::Object::newObject();
    auto uuid =
        qjspp::Locker::currentEngineChecked().newInstanceOfRaw(MinecraftModule::ScriptUUID, new mce::UUID(p->uuid));
    result.set("uuid", uuid);
    result.set("xuid", qjspp::String{p->xuid});
    result.set("name", qjspp::String{p->name});
    return result;
}

qjspp::Value PlayerInfo_FromUuid(qjspp::Arguments const& arguments) {
    if (arguments.length() != 1) {
        throw qjspp::JsException{qjspp::JsException::Type::TypeError, "fromUuid expects 1 argument"};
    }
    auto uuid = qjspp::bind::ConvertToCpp<mce::UUID*>(arguments[0]);
    if (!uuid) {
        throw qjspp::JsException{qjspp::JsException::Type::TypeError, "fromUuid expects a UUID argument"};
    }
    return PlayerInfo_CopyValue(PlayerInfo::getInstance().fromUuid(*uuid));
}
qjspp::Value PlayerInfo_FromXuid(qjspp::Arguments const& arguments) {
    if (arguments.length() != 1 || !arguments[0].isString()) {
        throw qjspp::JsException{qjspp::JsException::Type::TypeError, "fromXuid expects a string argument"};
    }
    return PlayerInfo_CopyValue(PlayerInfo::getInstance().fromXuid(arguments[0].asString().value()));
}
qjspp::Value PlayerInfo_FromName(qjspp::Arguments const& arguments) {
    if (arguments.length() != 1 || !arguments[0].isString()) {
        throw qjspp::JsException{qjspp::JsException::Type::TypeError, "fromName expects a string argument"};
    }
    return PlayerInfo_CopyValue(PlayerInfo::getInstance().fromName(arguments[0].asString().value()));
}

qjspp::bind::meta::ClassDefine const LeviLaminaModule::ScriptPlayerInfo = qjspp::bind::defineClass<void>("PlayerInfo")
                                                                              .function("fromUuid", PlayerInfo_FromUuid)
                                                                              .function("fromXuid", PlayerInfo_FromXuid)
                                                                              .function("fromName", PlayerInfo_FromName)
                                                                              .build();


} // namespace plotx::script::modules