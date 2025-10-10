#pragma once
#include "qjspp/Definitions.hpp"


namespace plotx::script::modules {


struct LeviLaminaDef {
    LeviLaminaDef() = delete;

    // === Form API ===
    static qjspp::ClassDefine const SimpleFormDef_;

    static qjspp::ClassDefine const CustomFormDef_;

    static qjspp::ClassDefine const ModalFormDef_;

    static qjspp::EnumDefine const ModalFormSelectedButtonDef_;

    // === Event API ===
    static qjspp::ClassDefine const EventBusDef_;

    static qjspp::ClassDefine const EventDef_;

    static qjspp::ClassDefine const CancellableEventDef_;

    static qjspp::ClassDefine const PlayerJoinEventDef_;

    static qjspp::EnumDefine const EventPriorityDef_;

    // === Data API ===
    static qjspp::ClassDefine const KeyValueDBDef_;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules
