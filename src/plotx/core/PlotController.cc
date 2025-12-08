#include "PlotController.hpp"

#include "PlotHandle.hpp"
#include "PlotRegistry.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/math/PlotCoord.hpp"
#include "plotx/utils/MessageUtils.hpp"

#include "ll/api/i18n/I18n.h"
#include "plotx/gui/BuyPlotGui.hpp"
#include "plotx/gui/PlotManagerGui.hpp"
#include "plotx/utils/StringUtils.hpp"

#include <mc/world/level/Level.h>
#include <mc/world/level/dimension/Dimension.h>

namespace plotx {

struct PlotController::Impl {
    PlotRegistry& registry;
    PlotX&        mod;
};

using ll::i18n_literals::operator""_tr;
using ll::i18n_literals::operator""_trl;

PlotController::PlotController(PlotRegistry& registry, PlotX& mod) : impl(std::make_unique<Impl>(registry, mod)) {}
PlotController::~PlotController() {}

void PlotController::teleportUnownedPlot(Player& player) const {
    if (auto coord = impl->registry.findUnownedPlot()) {
        // TODO: impl
    } else {
        message_utils::sendText<message_utils::LogLevel::Error>(
            player,
            "未找到最近的无主地皮"_trl(player.getLocaleCode())
        );
    }
}
void PlotController::teleportToPlot(Player& player, std::shared_ptr<PlotHandle> handle) {
    // TODO
}
void PlotController::sendPlayerCurrentPlot(Player& player) const {
    auto coord = PlotCoord{player.getPosition()};
    if (!coord.isValid()) {
        message_utils::sendText<message_utils::LogLevel::Error>(
            player,
            "您当前所在的位置不是地皮"_trl(player.getLocaleCode())
        );
        return;
    }
    if (impl->registry.hasPlot(coord)) {
        PlotManagerGUI::sendTo(player, impl->registry.getPlot(coord));
    } else {
        BuyPlotGUI::sendTo(player, std::move(coord));
    }
}

void PlotController::switchPlayerDimension(Player& player) const {
    if (player.getDimensionId() == PlotX::getDimensionId()) {
        player.teleport(player.getExpectedSpawnPosition(), player.getExpectedSpawnDimensionId(), player.getRotation());
    } else {
        auto& level = player.getLevel();
        if (auto dimension = level.getDimension(PlotX::getDimensionId()).lock()) {
            player.teleport(dimension->getSpawnPos(), dimension->getDimensionId(), player.getRotation());
        } else {
            impl->mod.getLogger().error("switch dimension failed, dimension is null");
            message_utils::sendText<message_utils::LogLevel::Error>(player, "切换维度失败"_trl(player.getLocaleCode()));
        }
    }
}
bool PlotController::changePlotName(Player& player, std::shared_ptr<PlotHandle> handle, std::string newName) {
    if (string_utils::length(newName) > 32) {
        message_utils::sendText<message_utils::LogLevel::Error>(player, "地皮名称过长"_trl(player.getLocaleCode()));
        return false;
    }
    handle->setName(newName);
    return true;
}

void PlotController::claimPlot(Player& player, PlotCoord coord) {
    // TODO: impl
}
void PlotController::buyPlotFromPlayer(Player& player, std::shared_ptr<PlotHandle> handle) {
    // TODO
}


} // namespace plotx