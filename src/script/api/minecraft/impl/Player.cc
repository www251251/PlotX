#include "script/api/Helper.hpp"
#include "script/api/minecraft/defs.hpp"


namespace plotx::script::api::inline minecraft {

qjspp::ClassDefine const PlayerDef_ =
    qjspp::defineClass<Player>("Player")
        .disableConstructor()
        .instanceProperty(
            "uuid",
            [](void* inst, qjspp::Arguments const& args) {
                return args.engine()->newInstanceOfView(
                    UUIDDef_,
                    const_cast<mce::UUID*>(&static_cast<Player*>(inst)->getUuid()),
                    args.thiz() // 关联生命周期
                );
            }
        )
        .instanceProperty(
            "realName",
            [](void* inst, qjspp::Arguments const&) { return qjspp::String{static_cast<Player*>(inst)->getRealName()}; }
        )
        .instanceMethod("sendMessage", &Player::sendMessage)
        .instanceMethod("isOperator", &Player::isOperator)
        .instanceMethod("getLocaleCode", &Player::getLocaleCode)
        .build();

} // namespace plotx::script::api::inline minecraft
