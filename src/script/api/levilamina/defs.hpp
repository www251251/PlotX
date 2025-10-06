#pragma once
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"


namespace plotx::script::api::inline levilamina {

// === Form API ===
extern qjspp::ClassDefine const SimpleFormDef_;

extern qjspp::ClassDefine const CustomFormDef_;

extern qjspp::ClassDefine const ModalFormDef_;
extern qjspp::EnumDefine const  ModalFormSelectedButtonDef_;


// === Event API ===
extern qjspp::ClassDefine const EventBusDef_;

extern qjspp::ClassDefine const EventDef_;

extern qjspp::ClassDefine const CancellableEventDef_;

extern qjspp::ClassDefine const PlayerJoinEventDef_;

extern qjspp::EnumDefine const EventPriorityDef_;


// === Data API ===
extern qjspp::ClassDefine const KeyValueDBDef_;


} // namespace plotx::script::api::inline levilamina
