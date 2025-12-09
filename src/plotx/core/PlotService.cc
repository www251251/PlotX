#include "PlotService.hpp"

#include "PlotHandle.hpp"
#include "PlotRegistry.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/math/PlotCoord.hpp"
#include "plotx/utils/MessageUtils.hpp"

#include "ll/api/i18n/I18n.h"
#include "plotx/events/PlayerChangePlotNameEvent.hpp"
#include "plotx/gui/BuyPlotGui.hpp"
#include "plotx/gui/PlotManagerGui.hpp"
#include "plotx/utils/StringUtils.hpp"

#include <ll/api/event/Event.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/service/Bedrock.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/dimension/Dimension.h>

namespace plotx {

struct PlotService::Impl {
    PlotRegistry& registry;
    PlotX&        mod;
};

using ll::i18n_literals::operator""_tr;
using ll::i18n_literals::operator""_trl;

PlotService::PlotService(PlotRegistry& registry, PlotX& mod) : impl(std::make_unique<Impl>(registry, mod)) {}
PlotService::~PlotService() {}


void PlotService::teleportUnownedPlot(Player& player) const {
    if (auto coord = impl->registry.findUnownedPlot()) {
        handleTeleportToPlot(player, coord->min.x, coord->min.z);
    } else {
        message_utils::sendText<message_utils::LogLevel::Error>(
            player,
            "未找到最近的无主地皮"_trl(player.getLocaleCode())
        );
    }
}

void PlotService::teleportToPlot(Player& player, std::shared_ptr<PlotHandle> handle) const {
    auto& min = handle->getCoord().min;
    handleTeleportToPlot(player, min.x, min.z);
}

void PlotService::showPlotGUIFor(Player& player) const {
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

void PlotService::switchPlayerDimension(Player& player) const {
    if (player.getDimensionId() == PlotX::getDimensionId()) {
        player.teleport(player.getExpectedSpawnPosition(), player.getExpectedSpawnDimensionId(), player.getRotation());
    } else {
        if (auto dimension = PlotX::getPlotDimension()) {
            player.teleport(dimension->getSpawnPos(), dimension->getDimensionId(), player.getRotation());
        } else {
            impl->mod.getLogger().error("switch dimension failed, dimension is null");
            message_utils::sendText<message_utils::LogLevel::Error>(player, "切换维度失败"_trl(player.getLocaleCode()));
        }
    }
}

bool PlotService::changePlotName(Player& player, std::shared_ptr<PlotHandle> handle, std::string newName) {
    if (string_utils::length(newName) > 32) {
        message_utils::sendText<message_utils::LogLevel::Error>(player, "地皮名称过长"_trl(player.getLocaleCode()));
        return false;
    }

    auto event = PlayerChangePlotNameEvent{player, handle, newName};
    ll::event::EventBus::getInstance().publish(event);
    if (event.isCancelled()) {
        return false;
    }

    handle->setName(newName);
    return true;
}

void PlotService::claimPlot(Player& player, PlotCoord coord) {
    // TODO: impl
}

void PlotService::buyPlotFromPlayer(Player& player, std::shared_ptr<PlotHandle> handle) {
    // TODO
}

void PlotService::handleTeleportToPlot(Player& player, int x, int z) const {
    if (auto dimension = PlotX::getPlotDimension()) {
        Vec3 position{x + 0.5f, dimension->getSpawnYPosition(), z + 0.5f};
        player.teleport(position, dimension->getDimensionId(), player.getRotation());
    } else {
        impl->mod.getLogger().error("handleTeleportToPlot failed, dimension is null");
        message_utils::sendText<message_utils::LogLevel::Error>(
            player,
            "传送失败，找不到地皮维度"_trl(player.getLocaleCode())
        );
    }
}


} // namespace plotx