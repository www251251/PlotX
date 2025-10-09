#include "ll/api/form/CustomForm.h"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/Defines.hpp"


namespace plotx::script::api::levilamina {

using namespace ll::form;

qjspp::ClassDefine const Defines::CustomForm =
    qjspp::defineClass<CustomForm>("CustomForm")
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

} // namespace plotx::script::api::levilamina