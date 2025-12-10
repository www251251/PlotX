#pragma once
#include "ll/api/event/Event.h"
#include "mc/deps/core/math/Vec3.h"
#include "plotx/Global.hpp"
#include "plotx/math/PlotCoord.hpp"

class Player;

namespace plotx::event {


/**
 * 玩家在地皮移动事件
 */
class IPlayerOnPlotMoveEvent : public ll::event::Event {
    Player*          player_;           // 玩家
    Vec3 const&      currentPos_;       // 当前位置
    int              currentDimId_;     // 当前维度
    PlotCoord const& currentPlotCoord_; // 当前地皮坐标
    int              lastDimId_;        // 上一个维度
    PlotCoord const& lastPlotCoord_;    // 上一个地皮坐标

public:
    PXAPI explicit IPlayerOnPlotMoveEvent(
        Player*          player,
        Vec3 const&      currentPos,
        int              currentDimId,
        PlotCoord const& currentPlotCoord,
        int              lastDimId,
        PlotCoord const& lastPlotCoord
    );

    PXNDAPI Player* getPlayer() const;

    PXNDAPI Vec3 const& getCurrentPos() const;

    PXNDAPI int getCurrentDimId() const;

    PXNDAPI PlotCoord const& getCurrentPlotCoord() const;

    PXNDAPI int getLastDimId() const;

    PXNDAPI PlotCoord const& getLastPlotCoord() const;
};


} // namespace plotx::event