#pragma once
#include <memory>

class Player;
class Vec3;

namespace plotx {

class PlotCoord;

/**
 * @brief 事件驱动
 * @note 驱动地皮的进出事件
 */
class PlotEventDriven {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    PlotEventDriven();
    ~PlotEventDriven();

    void tick() const;
    void updateTip(Player* player, Vec3 const& pos, PlotCoord const& coord) const;
};


} // namespace plotx