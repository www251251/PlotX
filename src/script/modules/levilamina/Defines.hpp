#pragma once
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"


namespace plotx::script::api::levilamina {


struct Defines {
    Defines() = delete;

    // === Form API ===
    static qjspp::ClassDefine const SimpleForm;

    static qjspp::ClassDefine const CustomForm;

    static qjspp::ClassDefine const ModalForm;

    static qjspp::EnumDefine const ModalFormSelectedButton;

    // === Event API ===
    static qjspp::ClassDefine const EventBus;

    static qjspp::ClassDefine const Event;

    static qjspp::ClassDefine const CancellableEvent;

    static qjspp::ClassDefine const PlayerJoinEvent;

    static qjspp::EnumDefine const EventPriority;

    // === Data API ===
    static qjspp::ClassDefine const KeyValueDB;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::api::levilamina
