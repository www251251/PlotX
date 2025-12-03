#pragma once
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "qjspp/bind/meta/EnumDefine.hpp"
#include "qjspp/bind/meta/ModuleDefine.hpp"


namespace plotx::script::modules {


struct LeviLaminaModule {
    LeviLaminaModule() = delete;

    // === Form API ===
    static qjspp::bind::meta::ClassDefine const ScriptSimpleForm;

    static qjspp::bind::meta::ClassDefine const ScriptCustomForm;

    static qjspp::bind::meta::ClassDefine const ScriptModalForm;

    static qjspp::bind::meta::EnumDefine const ScriptModalFormSelectedButton;

    // === Event API ===
    static qjspp::bind::meta::ClassDefine const ScriptEventBus;

    static qjspp::bind::meta::ClassDefine const ScriptEvent;

    static qjspp::bind::meta::ClassDefine const ScriptCancellableEvent;

    static qjspp::bind::meta::ClassDefine const ScriptPlayerJoinEvent;

    static qjspp::bind::meta::EnumDefine const ScriptEventPriority;

    // === Data API ===
    static qjspp::bind::meta::ClassDefine const ScriptKeyValueDB;

    static qjspp::bind::meta::ClassDefine const ScriptLogger;
    static qjspp::bind::meta::EnumDefine const  ScriptLogLevel;

    static qjspp::bind::meta::ClassDefine const ScriptPlayerInfo;

    static qjspp::bind::meta::ClassDefine const ScriptI18n;

    // === Command API ===
    static qjspp::bind::meta::ClassDefine const ScriptCommandRegistrar;
    static qjspp::bind::meta::ClassDefine const ScriptCommandHandle;
    static qjspp::bind::meta::ClassDefine const ScriptRuntimeOverload;
    static qjspp::bind::meta::EnumDefine const  ScriptCommandParamKind;

    static qjspp::bind::meta::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules
