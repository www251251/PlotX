#include "ll/api/form/ModalForm.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/Defines.hpp"


namespace plotx::script::api::levilamina {

using namespace ll::form;

qjspp::ClassDefine const Defines::ModalForm = qjspp::defineClass<ModalForm>("ModalForm")
                                                      .constructor<>()
                                                      .instanceMethod("setTitle", &ModalForm::setTitle)
                                                      .instanceMethod("setContent", &ModalForm::setContent)
                                                      .instanceMethod("setUpperButton", &ModalForm::setUpperButton)
                                                      .instanceMethod("setLowerButton", &ModalForm::setLowerButton)
                                                      .instanceMethod("sendTo", &ModalForm::sendTo)
                                                      .instanceMethod("sendUpdate", &ModalForm::sendUpdate)
                                                      .build();

qjspp::EnumDefine const Defines::ModalFormSelectedButton =
    qjspp::defineEnum<ModalFormSelectedButton>("ModalFormSelectedButton")
        .value("Upper", ModalFormSelectedButton::Upper)
        .value("Lower", ModalFormSelectedButton::Lower)
        .build();

} // namespace plotx::script::api::levilamina