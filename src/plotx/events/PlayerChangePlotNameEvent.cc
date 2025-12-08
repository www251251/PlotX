#include "PlayerChangePlotNameEvent.hpp"

namespace plotx {

PlayerChangePlotNameEvent::PlayerChangePlotNameEvent(
    Player&                     player,
    std::shared_ptr<PlotHandle> handle,
    std::string&                newName
)
: Cancellable(player),
  handle_(std::move(handle)),
  newName_(newName) {}

std::shared_ptr<PlotHandle> PlayerChangePlotNameEvent::getPlotHandle() const { return handle_; }

std::string& PlayerChangePlotNameEvent::getNewName() const { return newName_; }

} // namespace plotx