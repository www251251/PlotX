#include "PlotEventDriven.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/events/PlayerEnterPlotEvent.hpp"
#include "plotx/events/PlayerLeavePlotEvent.hpp"
#include "plotx/math/PlotCoord.hpp"

#include "ll/api/chrono/GameChrono.h"
#include "ll/api/coro/CoroTask.h"
#include "ll/api/coro/InterruptableSleep.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/ListenerBase.h"
#include "ll/api/event/player/PlayerDisconnectEvent.h"
#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/thread/ServerThreadExecutor.h"

#include "mc/deps/core/math/Vec3.h"
#include "mc/network/packet/TextPacket.h"
#include "mc/platform/UUID.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/actor/player/Player.h"

#include <unordered_map>
#include <vector>

namespace plotx {

struct PlotEventDriven::Impl {
    mutable std::shared_mutex                mutex;
    std::unordered_map<mce::UUID, int>       dimids_{};
    std::unordered_map<mce::UUID, PlotCoord> coords_{};
    std::vector<Player*>                     players_{};

    ll::event::ListenerPtr                        connectListener_{nullptr};
    ll::event::ListenerPtr                        disconnectListener_{nullptr};
    std::shared_ptr<ll::coro::InterruptableSleep> sleep_{nullptr};
    std::shared_ptr<std::atomic<bool>>            quit_{nullptr};
};

PlotEventDriven::PlotEventDriven() : impl(std::make_unique<Impl>()) {
    impl->connectListener_ = ll::event::EventBus::getInstance().emplaceListener<ll::event::PlayerJoinEvent>(
        [this](ll::event::PlayerJoinEvent& ev) {
            if (ev.self().isSimulatedPlayer()) {
                return;
            }
            std::unique_lock lk{impl->mutex};
            impl->players_.push_back(&ev.self());
        }
    );
    impl->disconnectListener_ = ll::event::EventBus::getInstance().emplaceListener<ll::event::PlayerDisconnectEvent>(
        [this](ll::event::PlayerDisconnectEvent& ev) {
            if (ev.self().isSimulatedPlayer()) {
                return;
            }
            std::unique_lock lk{impl->mutex};
            std::erase(impl->players_, &ev.self());
            impl->dimids_.erase(ev.self().getUuid());
            impl->coords_.erase(ev.self().getUuid());
        }
    );

    impl->sleep_ = std::make_shared<ll::coro::InterruptableSleep>();
    impl->quit_  = std::make_shared<std::atomic<bool>>(false);
    ll::coro::keepThis([quit = impl->quit_, sleep = impl->sleep_, this]() -> ll::coro::CoroTask<> {
        while (!quit->load()) {
            co_await sleep->sleepFor(ll::chrono::ticks{5});
            if (quit->load()) {
                break;
            }
            try {
                tick();
            } catch (...) {
                PlotX::getInstance().getSelf().getLogger().error(
                    "An exception occurred while ticking plot event driven. This may cause unexpected behavior."
                );
            }
        }
        co_return;
    }).launch(ll::thread::ServerThreadExecutor::getDefault());
}

PlotEventDriven::~PlotEventDriven() {
    impl->quit_->store(true);
    impl->sleep_->interrupt(true);
    ll::event::EventBus::getInstance().removeListener(impl->connectListener_);
    ll::event::EventBus::getInstance().removeListener(impl->disconnectListener_);
}

void PlotEventDriven::tick() const {
    auto&      bus       = ll::event::EventBus::getInstance();
    auto const plotDimId = PlotX::getDimensionId();

    std::shared_lock lk{impl->mutex};
    for (auto player : impl->players_) {
        auto const& uuid = player->getUuid();

        auto const& curPos   = player->getPosition();
        auto const  curDimId = player->getDimensionId();
        auto const  curPlot  = PlotCoord{curPos};

        auto& lastDimId = impl->dimids_[uuid];
        auto& lastPlot  = impl->coords_[uuid];

        // 维度变化
        if (curDimId != lastDimId) {
            // 当前不在地皮维度 & 上一次在地皮维度 & 上一次在地皮内 = 玩家离开地皮
            if (curDimId != plotDimId && lastDimId == plotDimId && lastPlot.isValid()) {
                bus.publish(event::PlayerLeavePlotEvent{player, curPos, curDimId, curPlot, lastDimId, lastPlot});
                if (!player->isCreative()) {
                    player->setAbility(AbilitiesIndex::MayFly, false);
                }
            }
            // 当前在地皮维度 & 上一次不在地皮维度 & 当前在地皮内 = 玩家进入地皮
            else if (curDimId == plotDimId && lastDimId != plotDimId && curPlot.isValid()) {
                bus.publish(event::PlayerEnterPlotEvent{player, curPos, curDimId, curPlot, lastDimId, lastPlot});
                if (!player->isCreative() && player->hasTag("vip")) {
                    if (auto plot = PlotX::getInstance().getPlotRegistry()->getPlot(curPlot);
                        plot && plot->isOwner(uuid)) {
                        player->setAbility(AbilitiesIndex::MayFly, true);
                    }
                }
            }
            lastDimId = curDimId;
            lastPlot  = curPlot;
            continue;
        }

        if (curDimId != plotDimId) {
            continue;
        }
        updateTip(player, curPos, curPlot);

        // 地皮变化
        if (curPlot != lastPlot) {
            // 上一次在地皮内 & 当前不在地皮内 = 玩家离开地皮
            if (lastPlot.isValid() && !curPlot.isValid()) {
                bus.publish(event::PlayerLeavePlotEvent{player, curPos, curDimId, curPlot, lastDimId, lastPlot});
                if (!player->isCreative()) {
                    player->setAbility(AbilitiesIndex::MayFly, false);
                }
            }
            // 上一次不在地皮内 & 当前在地皮内 = 玩家进入地皮
            else if (!lastPlot.isValid() && curPlot.isValid()) {
                bus.publish(event::PlayerEnterPlotEvent{player, curPos, curDimId, curPlot, lastDimId, lastPlot});
                if (!player->isCreative() && player->hasTag("vip")) {
                    if (auto plot = PlotX::getInstance().getPlotRegistry()->getPlot(curPlot);
                        plot && plot->isOwner(uuid)) {
                        player->setAbility(AbilitiesIndex::MayFly, true);
                    }
                }
            }
            lastPlot = curPlot;
        }
    }
}

void PlotEventDriven::updateTip(Player* player, Vec3 const& pos, PlotCoord const& coord) const {
    TextPacket pkt = TextPacket();
    pkt.mType      = TextPacketType::Tip;

    if (coord.isValid()) {
        pkt.mMessage = coord.toString();
    } else {
        pkt.mMessage = "Unknown";
    }

    pkt.sendTo(*player);
}


} // namespace plotx