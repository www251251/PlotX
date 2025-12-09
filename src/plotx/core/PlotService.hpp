#pragma once
#include <memory>
#include <string>


class Dimension;
class Player;
namespace plotx {
class PlotHandle;
class PlotCoord;
class PlotX;
class PlotRegistry;
} // namespace plotx

namespace plotx {

class PlotService {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    explicit PlotService(PlotRegistry& registry, PlotX& mod);
    ~PlotService();

    void teleportUnownedPlot(Player& player) const;

    void teleportToPlot(Player& player, std::shared_ptr<PlotHandle> handle) const;

    void showPlotGUIFor(Player& player) const;

    void switchPlayerDimension(Player& player) const;

    bool changePlotName(Player& player, std::shared_ptr<PlotHandle> handle, std::string newName);

    void claimPlot(Player& player, PlotCoord coord);

    void buyPlotFromPlayer(Player& player, std::shared_ptr<PlotHandle> handle);

private:
    void handleTeleportToPlot(Player& player, int x, int z) const;
};

} // namespace plotx
