#include "PlayerClaimPlotEvent.hpp"

#include "Helper.hpp"

namespace plotx {

PlotCoord const& PlayerClaimPlotEvent::getCoord() const { return coord_; }

IMPL_EVENT_EMITTER(PlayerClaimPlotEvent)

} // namespace plotx