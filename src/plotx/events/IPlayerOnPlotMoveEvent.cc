#include "IPlayerOnPlotMoveEvent.hpp"

namespace plotx::event {

IPlayerOnPlotMoveEvent::IPlayerOnPlotMoveEvent(
    Player*          player,
    Vec3 const&      currentPos,
    int              currentDimId,
    PlotCoord const& currentPlotCoord,
    int              lastDimId,
    PlotCoord const& lastPlotCoord
)
: player_(player),
  currentPos_(currentPos),
  currentDimId_(currentDimId),
  currentPlotCoord_(currentPlotCoord),
  lastDimId_(lastDimId),
  lastPlotCoord_(lastPlotCoord) {}

Player* IPlayerOnPlotMoveEvent::getPlayer() const { return player_; }

Vec3 const& IPlayerOnPlotMoveEvent::getCurrentPos() const { return currentPos_; }

int IPlayerOnPlotMoveEvent::getCurrentDimId() const { return currentDimId_; }

PlotCoord const& IPlayerOnPlotMoveEvent::getCurrentPlotCoord() const { return currentPlotCoord_; }

int IPlayerOnPlotMoveEvent::getLastDimId() const { return lastDimId_; }

PlotCoord const& IPlayerOnPlotMoveEvent::getLastPlotCoord() const { return lastPlotCoord_; }


} // namespace plotx::event