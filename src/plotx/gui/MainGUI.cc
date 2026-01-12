#include "MainGUI.hpp"
#include "PlotManagerGui.hpp"
#include "PlotPicker.hpp"
#include "PlotShopGui.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotService.hpp"

#include "ll/api/form/SimpleForm.h"
#include "ll/api/i18n/I18n.h"
#include "plotx/utils/FeedbackUtils.hpp"


namespace plotx {

void MainGUI::sendTo(Player& player) {
    using ll::i18n_literals::operator""_trl;
    auto localeCode = player.getLocaleCode();

    auto fm = ll::form::SimpleForm{};
    fm.setTitle("PlotX - 菜单"_trl(localeCode));
    fm.setContent("请选择一个操作"_trl(localeCode));

    fm.appendButton(
        "切换维度\n(往返地皮维度)"_trl(localeCode),
        "textures/ui/realmsIcon",
        "path",
        [localeCode](Player& player) {
            if (auto exp = PlotX::getInstance().getService()->switchPlayerDimension(player)) {
                feedback_utils::notifySuccess(player, "切换维度成功"_trl(localeCode));
            } else {
                feedback_utils::sendError(player, exp.error());
            }
        }
    );
    fm.appendButton("管理脚下地皮"_trl(localeCode), "textures/ui/icon_recipe_item", "path", [](Player& pl) {
        if (auto exp = PlotX::getInstance().getService()->showPlotGUIFor(pl); !exp) {
            feedback_utils::sendError(pl, exp.error());
        }
    });
    fm.appendButton("管理地皮"_trl(localeCode), "textures/ui/icon_recipe_nature", "path", [](Player& pl) {
        PlotPicker::sendTo(pl, [](Player& player, std::shared_ptr<PlotHandle> handle) {
            PlotManagerGUI::sendTo(player, handle);
        });
    });
    fm.appendButton("地皮商店"_trl(localeCode), "textures/ui/store_home_icon", "path", [](Player& pl) {
        PlotShopGUI::sendTo(pl);
    });
    fm.appendButton("关闭"_trl(localeCode));
    fm.sendTo(player);
}

} // namespace plotx