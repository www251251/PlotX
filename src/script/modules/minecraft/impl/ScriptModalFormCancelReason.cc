#include "mc/network/packet/ModalFormCancelReason.h"
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <qjspp/bind/builder/EnumDefineBuilder.hpp>


namespace plotx::script::modules {

qjspp::bind::meta::EnumDefine const MinecraftModule::ScriptModalFormCancelReason =
    qjspp::bind::defineEnum<::ModalFormCancelReason>("ModalFormCancelReason")
        .value("UserClosed", ModalFormCancelReason::UserClosed)
        .value("UserBusy", ModalFormCancelReason::UserBusy)
        .build();

}