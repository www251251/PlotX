#pragma once
#include "qjspp/Definitions.hpp"


namespace plotx::script::modules {


struct LeviLaminaModule {
    LeviLaminaModule() = delete;

    // === Form API ===
    static qjspp::ClassDefine const ScriptSimpleForm;

    static qjspp::ClassDefine const ScriptCustomForm;

    static qjspp::ClassDefine const ScriptModalForm;

    static qjspp::EnumDefine const ScriptModalFormSelectedButton;

    // === Event API ===
    static qjspp::ClassDefine const ScriptEventBus;

    static qjspp::ClassDefine const ScriptEvent;

    static qjspp::ClassDefine const ScriptCancellableEvent;

    static qjspp::ClassDefine const ScriptPlayerJoinEvent;

    static qjspp::EnumDefine const ScriptEventPriority;

    // === Data API ===
    static qjspp::ClassDefine const ScriptKeyValueDB;

    static qjspp::ClassDefine const ScriptLogger;
    static qjspp::EnumDefine const  ScriptLogLevel;

    static qjspp::ClassDefine const ScriptPlayerInfo;

    // === Command API ===
    static qjspp::ClassDefine const ScriptCommandRegistrar;
    static qjspp::ClassDefine const ScriptCommandHandle;
    static qjspp::ClassDefine const ScriptRuntimeOverload;
    static qjspp::EnumDefine const  ScriptCommandParamKind;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules
