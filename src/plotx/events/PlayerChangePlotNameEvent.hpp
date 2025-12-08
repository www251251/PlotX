#pragma once
#include <ll/api/event/Cancellable.h>
#include <ll/api/event/player/PlayerEvent.h>

#include <memory>
#include <string>


class Player;
namespace plotx {
class PlotHandle;

class PlayerChangePlotNameEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    std::shared_ptr<PlotHandle> handle_;
    std::string&                newName_;

public:
    explicit PlayerChangePlotNameEvent(Player& player, std::shared_ptr<PlotHandle> handle, std::string& newName);

    [[nodiscard]] std::shared_ptr<PlotHandle> getPlotHandle() const;

    [[nodiscard]] std::string& getNewName() const;
};

} // namespace plotx
