#include "PlotService.hpp"

#include "Config.hpp"
#include "PlotHandle.hpp"
#include "PlotRegistry.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/math/PlotCoord.hpp"
#include "plotx/utils/FeedbackUtils.hpp"

#include "ll/api/i18n/I18n.h"
#include "plotx/events/PlayerChangePlotNameEvent.hpp"
#include "plotx/events/PlayerClaimPlotEvent.hpp"
#include "plotx/events/PlayerModifyPlotMemberEvent.hpp"
#include "plotx/events/PlotOwnershipTransferEvent.hpp"
#include "plotx/gui/BuyPlotGui.hpp"
#include "plotx/gui/PlotManagerGui.hpp"
#include "plotx/infra/PlotResult.hpp"
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


ll::Expected<> PlotService::teleportUnownedPlot(Player& player) const {
    if (auto coord = impl->registry.findUnownedPlot()) {
        return handleTeleportToPlot(player, coord->min.x, coord->min.z);
    }
    return makeUserError("未找到最近的无主地皮"_trl(player.getLocaleCode()));
}

ll::Expected<> PlotService::teleportToPlot(Player& player, std::shared_ptr<PlotHandle> handle) const {
    auto& min = handle->getCoord().min;
    return handleTeleportToPlot(player, min.x, min.z);
}

ll::Expected<> PlotService::showPlotGUIFor(Player& player) const {
    auto coord = PlotCoord{player.getPosition()};
    if (!coord.isValid()) {
        return makeUserError("您当前所在的位置不是地皮"_trl(player.getLocaleCode()));
    }
    if (impl->registry.hasPlot(coord)) {
        PlotManagerGUI::sendTo(player, impl->registry.getPlot(coord));
    } else {
        BuyPlotGUI::sendTo(player, std::move(coord));
    }
    return {};
}

ll::Expected<> PlotService::switchPlayerDimension(Player& player) const {
    if (player.getDimensionId() == PlotX::getDimensionId()) {
        player.teleport(player.getExpectedSpawnPosition(), player.getExpectedSpawnDimensionId(), player.getRotation());
        return {};
    }
    if (auto dimension = PlotX::getPlotDimension()) {
        player.teleport(dimension->getSpawnPos(), dimension->getDimensionId(), player.getRotation());
        return {};
    }
    return makeSystemError("切换维度失败"_trl(player.getLocaleCode()), "switch dimension failed, dimension is null");
}

ll::Expected<> PlotService::changePlotName(Player& player, std::shared_ptr<PlotHandle> handle, std::string newName) {
    if (string_utils::length(newName) > 32) {
        return makeUserError("地皮名称过长"_trl(player.getLocaleCode()));
    }

    auto event = PlayerChangePlotNameEvent{player, handle, newName};
    ll::event::EventBus::getInstance().publish(event);
    if (event.isCancelled()) {
        return makeUserError("操作被其它插件拦截"_trl(player.getLocaleCode()));
    }

    handle->setName(newName);
    return {};
}

ll::Expected<>
PlotService::modifyPlotMember(Player& player, std::shared_ptr<PlotHandle> handle, mce::UUID const& target, bool isAdd) {
    auto localeCode = player.getLocaleCode();
    if (target == mce::UUID::EMPTY()) {
        return makeUserError("无效的玩家UUID"_trl(localeCode));
    }

    static_assert((bool)PlayerModifyPlotMemberEvent::ModifyType::Add == true);
    auto event = PlayerModifyPlotMemberEvent{
        player,
        handle,
        target,
        static_cast<PlayerModifyPlotMemberEvent::ModifyType>(isAdd)
    };
    ll::event::EventBus::getInstance().publish(event);
    if (event.isCancelled()) {
        return makeUserError("操作被其它插件拦截"_trl(localeCode));
    }

    isAdd ? handle->addMember(target) : handle->removeMember(target);
    return {};
}

ll::Expected<> PlotService::claimPlot(Player& player, PlotCoord coord) {
    auto localeCode = player.getLocaleCode();
    if (!coord.isValid()) {
        return makeUserError("您当前所在的位置不是地皮"_trl(localeCode));
    }
    if (impl->registry.hasPlot(coord)) {
        return makeUserError("该地皮已被认领，您不能重复认领"_trl(localeCode));
    }
    if (auto economy = PlotX::getInstance().getEconomy();
        economy && gConfig_.plot.sellPrice > 0 && !economy->reduce(player.getUuid(), gConfig_.plot.sellPrice)) {
        return makeUserError("您的经济余额不足");
    }

    auto event = PlayerClaimPlotEvent{player, coord};
    ll::event::EventBus::getInstance().publish(event);
    if (event.isCancelled()) {
        return makeUserError("操作被其它插件拦截"_trl(localeCode));
    }

    auto handle = PlotHandle::make(coord, player.getUuid());
    if (impl->registry.addPlot(handle)) {
        return {};
    }
    return makeSystemError(
        "认领地皮失败，请稍后再试"_trl(localeCode),
        "claim plot failed, PlotRegistry addPlot failed"
    );
}

ll::Expected<> PlotService::transferPlotTo(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();
    if (handle->isOwner(player.getUuid())) {
        return makeUserError("您已经是地皮主人了"_trl(localeCode));
    }

    auto event = PlotOwnershipTransferEvent{player, handle};
    ll::event::EventBus::getInstance().publish(event);
    if (event.isCancelled()) {
        return makeUserError("操作被其它插件拦截"_trl(localeCode));
    }

    handle->setOwner(player.getUuid());
    return {};
}

ll::Expected<> PlotService::handleTeleportToPlot(Player& player, int x, int z) const {
    if (auto dimension = PlotX::getPlotDimension()) {
        Vec3 position{x + 0.5f, dimension->getSpawnYPosition(), z + 0.5f};
        player.teleport(position, dimension->getDimensionId(), player.getRotation());
        return {};
    }
    return makeSystemError(
        "传送失败，找不到地皮维度"_trl(player.getLocaleCode()),
        "teleport to plot failed, dimension is null"
    );
}


} // namespace plotx