#include "PlotEventDriven.hpp"
#include "ll/api/chrono/GameChrono.h"
#include "ll/api/coro/CoroTask.h"
#include "ll/api/coro/InterruptableSleep.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/ListenerBase.h"
#include "ll/api/event/player/PlayerDisconnectEvent.h"
#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/thread/ServerThreadExecutor.h"
#include "mc/network/packet/TextPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/actor/player/Player.h"
#include "plotx/PlotX.hpp"
#include "plotx/events/PlayerEnterPlotEvent.hpp"
#include "plotx/events/PlayerLeavePlotEvent.hpp"
#include "plotx/math/PlotCoord.hpp"
#include <vector>


namespace plotx {


PlotEventDriven::PlotEventDriven() {
    connectListener_ = ll::event::EventBus::getInstance().emplaceListener<ll::event::PlayerJoinEvent>(
        [this](ll::event::PlayerJoinEvent& ev) {
            if (ev.self().isSimulatedPlayer()) {
                return;
            }
            players_.push_back(&ev.self());
        }
    );
    disconnectListener_ = ll::event::EventBus::getInstance().emplaceListener<ll::event::PlayerDisconnectEvent>(
        [this](ll::event::PlayerDisconnectEvent& ev) {
            if (ev.self().isSimulatedPlayer()) {
                return;
            }
            players_.erase(std::remove(players_.begin(), players_.end(), &ev.self()), players_.end());
            dimids_.erase(ev.self().getUuid());
            coords_.erase(ev.self().getUuid());
        }
    );

    sleep_ = std::make_shared<ll::coro::InterruptableSleep>();
    quit_  = std::make_shared<std::atomic<bool>>(false);
    ll::coro::keepThis([quit = quit_, sleep = sleep_, this]() -> ll::coro::CoroTask<> {
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
    quit_->store(true);
    sleep_->interrupt(true);
    ll::event::EventBus::getInstance().removeListener(connectListener_);
    ll::event::EventBus::getInstance().removeListener(disconnectListener_);
}

void PlotEventDriven::tick() {
    auto& bus  = ll::event::EventBus::getInstance();
    auto  iter = players_.begin();
    while (iter != players_.end()) {
        try {
            auto player = *iter;

            auto const& uuid = player->getUuid();

            auto const& curPos  = player->getPosition();
            auto const  curDim  = player->getDimensionId();
            auto const  curPlot = PlotCoord{curPos};

            auto& lastDim  = dimids_[uuid];
            auto& lastPlot = coords_[uuid];

            auto const plotDim = PlotX::getDimensionId();

            // 维度变化
            if (curDim != lastDim) {
                // 当前不在地皮维度 & 上一次在地皮维度 & 上一次在地皮内
                if (curDim != plotDim && lastDim == plotDim && lastPlot.isValid()) {
                    // 玩家离开地皮
                    bus.publish(event::PlayerLeavePlotEvent{player, curPos, curDim, curPlot, lastDim, lastPlot});
                }
                // 当前在地皮维度 & 上一次不在地皮维度 & 当前在地皮内
                else if (curDim == plotDim && lastDim != plotDim && curPlot.isValid()) {
                    // 玩家进入地皮
                    bus.publish(event::PlayerEnterPlotEvent{player, curPos, curDim, curPlot, lastDim, lastPlot});
                }
                lastDim  = curDim;  // 更新维度缓存
                lastPlot = curPlot; // 更新地皮缓存
                ++iter;
                continue;
            }

            if (curDim != plotDim) {
                ++iter;
                continue;
            }
            updateTip(player, curPos, curPlot);

            // 地皮变化
            // 1. 离开地皮
            // 2. 进入地皮
            if (curPlot != lastPlot) {
                // 上一次在地皮内 & 当前不在地皮内
                if (lastPlot.isValid() && !curPlot.isValid()) {
                    // 玩家离开地皮
                    bus.publish(event::PlayerLeavePlotEvent{player, curPos, curDim, curPlot, lastDim, lastPlot});
                }
                // 上一次不在地皮内 & 当前在地皮内
                else if (!lastPlot.isValid() && curPlot.isValid()) {
                    // 玩家进入地皮
                    bus.publish(event::PlayerEnterPlotEvent{player, curPos, curDim, curPlot, lastDim, lastPlot});
                }
                lastPlot = curPlot; // 更新地皮缓存
            }

            ++iter;
        } catch (...) {
            iter = players_.erase(iter);
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