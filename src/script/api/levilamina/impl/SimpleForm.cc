#include "ll/api/form/SimpleForm.h"
#include "script/api/Helper.hpp"
#include "script/api/levilamina/defs.hpp"


namespace plotx::script::api::inline levilamina {

using namespace ll::form;

qjspp::ClassDefine const SimpleFormDef_ =
    qjspp::defineClass<SimpleForm>("SimpleForm")
        .constructor<>()
        .instanceMethod("setTitle", &SimpleForm::setTitle)
        .instanceMethod("setContent", &SimpleForm::setContent)
        .instanceMethod("appendHeader", &SimpleForm::appendHeader)
        .instanceMethod("appendLabel", &SimpleForm::appendLabel)
        .instanceMethod("appendDivider", &SimpleForm::appendDivider)
        .instanceMethod(
            "appendButton",
            static_cast<SimpleForm& (SimpleForm::*)(std::string const&,
                                                    std::string const&,
                                                    std::string const&,
                                                    SimpleForm::ButtonCallback)>(&SimpleForm::appendButton),
            static_cast<SimpleForm& (SimpleForm::*)(std::string const&, SimpleForm::ButtonCallback)>(
                &SimpleForm::appendButton
            )
        )
        .instanceMethod("sendTo", &SimpleForm::sendTo)
        .instanceMethod("sendUpdate", &SimpleForm::sendUpdate)
        .build();

} // namespace plotx::script::api::inline levilamina