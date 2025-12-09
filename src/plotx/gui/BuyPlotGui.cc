#include "BuyPlotGui.hpp"

#include "plotx/PlotX.hpp"
#include "plotx/core/PlotService.hpp"
#include "plotx/infra/Config.hpp"
#include "plotx/math/PlotCoord.hpp"

#include "ll/api/form/ModalForm.h"
#include <ll/api/i18n/I18n.h>

namespace plotx {

void BuyPlotGUI::sendTo(Player& player, PlotCoord coord) {
    using ll::i18n_literals::operator""_trl;
    auto localeCode = player.getLocaleCode();

    ll::form::ModalForm{
        "PlotX - 购买地皮"_trl(localeCode),
        "位置: {},{}\n价格: {}"_trl(localeCode, coord.x, coord.z, gConfig_.plot.sellPrice),
        "购买"_trl(localeCode),
        "关闭"_trl(localeCode)
    }
        .sendTo(player, [coord = std::move(coord)](Player& player, ll::form::ModalFormResult const& result, auto) {
            if (!result) {
                return;
            }
            if ((bool)result.value()) {
                PlotX::getInstance().getService()->claimPlot(player, coord);
            }
        });
}

} // namespace plotx