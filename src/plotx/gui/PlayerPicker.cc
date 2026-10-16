#include "PlayerPicker.hpp"

#include <ll/api/form/CustomForm.h>
#include <ll/api/form/SimpleForm.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/service/Bedrock.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/level/Level.h>

namespace plotx {

using namespace ll::i18n_literals;

void PlayerPicker::sendTo(Player& player, std::function<void(Player& self, mce::UUID picked)> const& callback) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 玩家选择器"_trl(localeCode));
    f.setContent("请选择一个玩家"_trl(localeCode));

    ll::service::getLevel()->forEachPlayer([&](Player& other) {
        f.appendButton(other.getRealName(), [callback, uuid = other.getUuid()](Player& player) {
            callback(player, uuid);
        });
        return true;
    });

    f.sendTo(player);
}

} // namespace plotx
