#include "PlotPicker.hpp"

#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"

#include <ll/api/form/CustomForm.h>
#include <ll/api/form/SimpleForm.h>
#include <ll/api/i18n/I18n.h>
#include <mc/world/actor/player/Player.h>

namespace plotx {

void PlotPicker::sendTo(Player& player, std::function<void(Player&, std::shared_ptr<PlotHandle>)> const& callback) {
    using namespace ll::i18n_literals;
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮选择器"_trl(localeCode));
    f.setContent("请选择一个地皮"_trl(localeCode));

    auto plots = PlotX::getInstance().getPlotRegistry()->getPlots(player.getUuid());
    for (auto const& plot : plots) {
        f.appendButton(plot->getName(), [callback, plot](Player& player) { callback(player, plot); });
    }
    f.sendTo(player);
}

} // namespace plotx
