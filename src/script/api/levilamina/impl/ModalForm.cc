#include "ll/api/form/ModalForm.h"
#include "script/api/Helper.hpp"
#include "script/api/levilamina/defs.hpp"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"


namespace plotx::script::api::inline levilamina {

using namespace ll::form;

qjspp::ClassDefine const ModalFormDef_ = qjspp::defineClass<ModalForm>("ModalForm")
                                             .constructor<>()
                                             .instanceMethod("setTitle", &ModalForm::setTitle)
                                             .instanceMethod("setContent", &ModalForm::setContent)
                                             .instanceMethod("setUpperButton", &ModalForm::setUpperButton)
                                             .instanceMethod("setLowerButton", &ModalForm::setLowerButton)
                                             .instanceMethod("sendTo", &ModalForm::sendTo)
                                             .instanceMethod("sendUpdate", &ModalForm::sendUpdate)
                                             .build();

qjspp::EnumDefine const ModalFormSelectedButtonDef_ =
    qjspp::defineEnum<ModalFormSelectedButton>("ModalFormSelectedButton")
        .value("Upper", ModalFormSelectedButton::Upper)
        .value("Lower", ModalFormSelectedButton::Lower)
        .build();

} // namespace plotx::script::api::inline levilamina