#include "ll/api/form/CustomForm.h"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>


namespace plotx::script::modules {

using namespace ll::form;

qjspp::bind::meta::ClassDefine const LeviLaminaModule::ScriptCustomForm =
    qjspp::bind::defineClass<ll::form::CustomForm>("CustomForm")
        .constructor<>()
        .instanceMethod("setTitle", &CustomForm::setTitle)
        .instanceMethod("setSubmitButton", &CustomForm::setSubmitButton)
        .instanceMethod("appendHeader", &CustomForm::appendHeader)
        .instanceMethod("appendLabel", &CustomForm::appendLabel)
        .instanceMethod("appendDivider", &CustomForm::appendDivider)
        .instanceMethod("appendInput", &CustomForm::appendInput)
        .instanceMethod("appendToggle", &CustomForm::appendToggle)
        .instanceMethod("appendDropdown", &CustomForm::appendDropdown)
        .instanceMethod("appendSlider", &CustomForm::appendSlider)
        .instanceMethod("appendStepSlider", &CustomForm::appendStepSlider)
        .instanceMethod("sendTo", &CustomForm::sendTo)
        .instanceMethod("sendUpdate", &CustomForm::sendUpdate)
        .instanceMethod("getFormData", &CustomForm::getFormData)
        .build();

} // namespace plotx::script::modules