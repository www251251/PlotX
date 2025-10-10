#include "ll/api/form/ModalForm.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaDef.hpp"


namespace plotx::script::modules {

using namespace ll::form;

qjspp::ClassDefine const LeviLaminaDef::ModalFormDef_ =
    qjspp::defineClass<ll::form::ModalForm>("ModalForm")
        .constructor<>()
        .instanceMethod("setTitle", &ModalForm::setTitle)
        .instanceMethod("setContent", &ModalForm::setContent)
        .instanceMethod("setUpperButton", &ModalForm::setUpperButton)
        .instanceMethod("setLowerButton", &ModalForm::setLowerButton)
        .instanceMethod("sendTo", &ModalForm::sendTo)
        .instanceMethod("sendUpdate", &ModalForm::sendUpdate)
        .build();

qjspp::EnumDefine const LeviLaminaDef::ModalFormSelectedButtonDef_ =
    qjspp::defineEnum<ll::form::ModalFormSelectedButton>("ModalFormSelectedButton")
        .value("Upper", ModalFormSelectedButton::Upper)
        .value("Lower", ModalFormSelectedButton::Lower)
        .build();

} // namespace plotx::script::modules