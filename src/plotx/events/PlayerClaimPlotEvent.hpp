#pragma once
#include "plotx/math/PlotCoord.hpp"


#include <ll/api/event/Cancellable.h>
#include <ll/api/event/player/PlayerEvent.h>

namespace plotx {
class PlotCoord;

class PlayerClaimPlotEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    PlotCoord const& coord_;

public:
    explicit PlayerClaimPlotEvent(Player& player, PlotCoord const& coord) : Cancellable(player), coord_(coord) {}

    PXNDAPI PlotCoord const& getCoord() const;
};

} // namespace plotx
