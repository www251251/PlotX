#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"


namespace plotx::script::modules {

qjspp::ClassDefine const MinecraftModule::ScriptPlayer =
    qjspp::defineClass<::Player>("Player")
        .disableConstructor()
        .instanceProperty(
            "uuid",
            [](void* inst, qjspp::Arguments const& args) {
                return args.engine()->newInstanceOfView(
                    ScriptUUID,
                    const_cast<mce::UUID*>(&static_cast<::Player*>(inst)->getUuid()),
                    args.thiz() // 关联生命周期
                );
            }
        )
        .instanceProperty(
            "realName",
            [](void* inst, qjspp::Arguments const&) {
                return qjspp::String{static_cast<::Player*>(inst)->getRealName()};
            }
        )
        .instanceMethod("sendMessage", &Player::sendMessage)
        .instanceMethod("isOperator", &Player::isOperator)
        .instanceMethod("getLocaleCode", &Player::getLocaleCode)
        .build();

} // namespace plotx::script::modules
