#include "ll/api/form/ModalForm.h"

#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include "qjspp/bind/builder/ClassDefineBuilder.hpp"
#include "qjspp/bind/builder/EnumDefineBuilder.hpp"
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "qjspp/bind/meta/EnumDefine.hpp"

namespace plotx::script::modules {

using namespace ll::form;

qjspp::bind::meta::ClassDefine const LeviLaminaModule::ScriptModalForm =
    qjspp::bind::defineClass<ll::form::ModalForm>("ModalForm")
        .constructor<>()
        .instanceMethod("setTitle", &ModalForm::setTitle)
        .instanceMethod("setContent", &ModalForm::setContent)
        .instanceMethod("setUpperButton", &ModalForm::setUpperButton)
        .instanceMethod("setLowerButton", &ModalForm::setLowerButton)
        .instanceMethod("sendTo", &ModalForm::sendTo)
        .instanceMethod("sendUpdate", &ModalForm::sendUpdate)
        .build();

qjspp::bind::meta::EnumDefine const LeviLaminaModule::ScriptModalFormSelectedButton =
    qjspp::bind::defineEnum<ll::form::ModalFormSelectedButton>("ModalFormSelectedButton")
        .value("Upper", ModalFormSelectedButton::Upper)
        .value("Lower", ModalFormSelectedButton::Lower)
        .build();

} // namespace plotx::script::modules