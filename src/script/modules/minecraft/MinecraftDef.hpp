#pragma once
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {


struct MinecraftDef {
    MinecraftDef() = delete;

    static qjspp::ClassDefine const PlayerDef_;

    static qjspp::ClassDefine const UUIDDef_;

    static qjspp::EnumDefine const ModalFormCancelReasonDef_;

    static qjspp::ClassDefine const BlockPosDef_;

    static qjspp::ClassDefine const Vec3Def_;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules