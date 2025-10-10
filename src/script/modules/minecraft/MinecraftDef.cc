#include "MinecraftDef.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {

qjspp::ModuleDefine const& MinecraftDef::getModuleDef() {
    static auto def = qjspp::defineModule("@minecraft")
                          .addClass(PlayerDef_)
                          .addClass(UUIDDef_)
                          .addEnum(ModalFormCancelReasonDef_)
                          .addClass(BlockPosDef_)
                          .addClass(Vec3Def_)
                          .build();
    return def;
}

} // namespace plotx::script::modules
