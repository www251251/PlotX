#include "mc/network/packet/ModalFormCancelReason.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "script/modules/minecraft/MinecraftDef.hpp"


namespace plotx::script::modules {

qjspp::EnumDefine const MinecraftDef::ModalFormCancelReasonDef_ =
    qjspp::defineEnum<::ModalFormCancelReason>("ModalFormCancelReason")
        .value("UserClosed", ModalFormCancelReason::UserClosed)
        .value("UserBusy", ModalFormCancelReason::UserBusy)
        .build();

}