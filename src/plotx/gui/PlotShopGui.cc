#include "PlotShopGui.hpp"

#include "PlotCommentGui.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/core/PlotService.hpp"
#include "plotx/utils/FeedbackUtils.hpp"

#include <ll/api/form/CustomForm.h>
#include <ll/api/form/ModalForm.h>
#include <ll/api/form/SimpleForm.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/service/PlayerInfo.h>
#include <mc/world/actor/player/Player.h>

namespace plotx {

using ll::i18n_literals::operator""_trl;

void PlotShopGUI::sendTo(Player& player) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮商店"_trl(localeCode));
    f.setContent("选择一个地皮进行购买或前往参观"_trl(localeCode));

    auto plots = PlotX::getInstance().getPlotRegistry()->getSellingPlots();
    for (auto& h : plots) {
        f.appendButton("{}\n价格: {}"_trl(localeCode, h->getName(), h->getSellingPrice()), [h](Player& player) {
            showPlot(player, h);
        });
    }
    f.sendTo(player);
}
void PlotShopGUI::showPlot(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();
    auto ownerInfo  = ll::service::PlayerInfo::getInstance().fromUuid(handle->getOwner());

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮商店"_trl(localeCode));
    f.setContent(
        "位置: {},{}\n所有者: {}\n名称: {}\n状态: {}"_trl(
            localeCode,
            handle->getCoord().x,
            handle->getCoord().z,
            ownerInfo ? ownerInfo->name : handle->getOwner().asString(),
            handle->getName(),
            handle->isForSale() ? "出售中 (价格:{})"_trl(localeCode, handle->getSellingPrice())
                                : "未出售"_trl(localeCode)
        )
    );
    f.appendButton("传送到此地皮"_trl(localeCode), "textures/ui/move", "path", [handle, ownerInfo](Player& player) {
        if (auto exp = PlotX::getInstance().getService()->teleportToPlot(player, handle)) {
            auto localeCode = player.getLocaleCode();
            feedback_utils::notifySuccess(
                player,
                "传送到地皮 {} 位置: {}, 所有者: {}"_trl(
                    localeCode,
                    handle->getName(),
                    handle->getCoord().min.toString(),
                    ownerInfo ? ownerInfo->name : handle->getOwner().asString()
                )
            );
        } else {
            feedback_utils::sendError(player, exp.error());
        }
    });
    f.appendButton("地皮评论"_trl(localeCode), "textures/ui/icon_sign", "path", [handle](Player& player) {
        PlotCommentGUI::sendTo(player, handle);
    });
    f.appendButton("购买此地皮"_trl(localeCode), "textures/ui/confirm", "path", [handle](Player& player) {
        confirmBuyPlot(player, handle);
    });
    f.sendTo(player);
}
void PlotShopGUI::confirmBuyPlot(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();
    auto ownerInfo  = ll::service::PlayerInfo::getInstance().fromUuid(handle->getOwner());
    ll::form::ModalForm{
        "PlotX - 地皮商店"_trl(localeCode),
        "确认购买玩家 {} 的地皮 {}？\n购买后您将获得此地皮的所有权"_trl(
            localeCode,
            ownerInfo ? ownerInfo->name : handle->getOwner().asString(),
            handle->getName()
        ),
        "确认"_trl(localeCode),
        "返回"_trl(localeCode)
    }
        .sendTo(player, [handle, localeCode](Player& player, ll::form::ModalFormResult const& result, auto) {
            if (!result) {
                return;
            }
            if ((bool)result.value()) {
                if (auto exp = PlotX::getInstance().getService()->transferPlotTo(player, handle)) {
                    feedback_utils::notifySuccess(
                        player,
                        "您已获得地皮 {}(位置 {},{}) 的所有权"_trl(
                            localeCode,
                            handle->getName(),
                            handle->getCoord().x,
                            handle->getCoord().z
                        )
                    );
                } else {
                    feedback_utils::sendError(player, exp.error());
                }
            } else {
                showPlot(player, handle);
            }
        });
}


} // namespace plotx