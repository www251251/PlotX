#pragma once
#include "plotx/Global.hpp"

#include <ll/api/event/Cancellable.h>
#include <ll/api/event/player/PlayerEvent.h>

namespace plotx {
class PlotHandle;
}
namespace plotx {

class PlotOwnershipTransferEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    std::shared_ptr<PlotHandle> handle_;

public:
    explicit PlotOwnershipTransferEvent(Player& player, std::shared_ptr<PlotHandle> handle)
    : Cancellable(player),
      handle_(handle) {}

    PXNDAPI std::shared_ptr<PlotHandle> getHandle() const;
};

} // namespace plotx
