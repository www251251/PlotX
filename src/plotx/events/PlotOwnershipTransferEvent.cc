#include "PlotOwnershipTransferEvent.hpp"

#include "Helper.hpp"

namespace plotx {

std::shared_ptr<PlotHandle> PlotOwnershipTransferEvent::getHandle() const { return handle_; }

IMPL_EVENT_EMITTER(PlotOwnershipTransferEvent)

} // namespace plotx