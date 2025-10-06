#include "mc/network/packet/ModalFormCancelReason.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "script/api/minecraft/defs.hpp"


namespace plotx::script::api::inline minecraft {

qjspp::EnumDefine const ModalFormCancelReasonDef_ = qjspp::defineEnum<ModalFormCancelReason>("ModalFormCancelReason")
                                                        .value("UserClosed", ModalFormCancelReason::UserClosed)
                                                        .value("UserBusy", ModalFormCancelReason::UserBusy)
                                                        .build();

}