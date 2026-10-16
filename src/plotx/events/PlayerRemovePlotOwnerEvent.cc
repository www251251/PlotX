#include "PlayerRemovePlotOwnerEvent.hpp"

#include "Helper.hpp"

namespace plotx {

std::shared_ptr<PlotHandle> PlayerRemovePlotOwnerEvent::getHandle() const { return handle_; }

IMPL_EVENT_EMITTER(PlayerRemovePlotOwnerEvent)

} // namespace plotx
