#include "ll/api/form/ModalForm.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"


namespace plotx::script::modules {

using namespace ll::form;

qjspp::ClassDefine const LeviLaminaModule::ScriptModalForm =
    qjspp::defineClass<ll::form::ModalForm>("ModalForm")
        .constructor<>()
        .instanceMethod("setTitle", &ModalForm::setTitle)
        .instanceMethod("setContent", &ModalForm::setContent)
        .instanceMethod("setUpperButton", &ModalForm::setUpperButton)
        .instanceMethod("setLowerButton", &ModalForm::setLowerButton)
        .instanceMethod("sendTo", &ModalForm::sendTo)
        .instanceMethod("sendUpdate", &ModalForm::sendUpdate)
        .build();

qjspp::EnumDefine const LeviLaminaModule::ScriptModalFormSelectedButton =
    qjspp::defineEnum<ll::form::ModalFormSelectedButton>("ModalFormSelectedButton")
        .value("Upper", ModalFormSelectedButton::Upper)
        .value("Lower", ModalFormSelectedButton::Lower)
        .build();

} // namespace plotx::script::modules