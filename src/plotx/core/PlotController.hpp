#pragma once
#include <memory>
#include <string>


namespace plotx {
class PlotCoord;
}
namespace plotx {
class PlotHandle;
}
class Player;
namespace plotx {
class PlotX;
class PlotRegistry;
} // namespace plotx

namespace plotx {

class PlotController {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    explicit PlotController(PlotRegistry& registry, PlotX& mod);
    ~PlotController();

    void teleportUnownedPlot(Player& player) const;

    void teleportToPlot(Player& player, std::shared_ptr<PlotHandle> handle);

    void sendPlayerCurrentPlot(Player& player) const;

    void switchPlayerDimension(Player& player) const;

    bool changePlotName(Player& player, std::shared_ptr<PlotHandle> handle, std::string newName);

    void claimPlot(Player& player, PlotCoord coord);

    void buyPlotFromPlayer(Player& player, std::shared_ptr<PlotHandle> handle);
};

} // namespace plotx
