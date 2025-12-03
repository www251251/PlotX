#include "ll/api/form/SimpleForm.h"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>


namespace plotx::script::modules {

using namespace ll::form;

qjspp::bind::meta::ClassDefine const LeviLaminaModule::ScriptSimpleForm =
    qjspp::bind::defineClass<ll::form::SimpleForm>("SimpleForm")
        .constructor<>()
        .instanceMethod("setTitle", &SimpleForm::setTitle)
        .instanceMethod("setContent", &SimpleForm::setContent)
        .instanceMethod("appendHeader", &SimpleForm::appendHeader)
        .instanceMethod("appendLabel", &SimpleForm::appendLabel)
        .instanceMethod("appendDivider", &SimpleForm::appendDivider)
        .instanceMethod(
            "appendButton",
            static_cast<ll::form::SimpleForm& (SimpleForm::*)(std::string const&,
                                                              std::string const&,
                                                              std::string const&,
                                                              SimpleForm::ButtonCallback)>(&SimpleForm::appendButton),
            static_cast<ll::form::SimpleForm& (SimpleForm::*)(std::string const&, SimpleForm::ButtonCallback)>(
                &SimpleForm::appendButton
            )
        )
        .instanceMethod("sendTo", &SimpleForm::sendTo)
        .instanceMethod("sendUpdate", &SimpleForm::sendUpdate)
        .build();

} // namespace plotx::script::modules