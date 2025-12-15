#pragma once
#include <complex.h>
#include <memory>
#include <optional>


class Player;

namespace plotx {
class PlotHandle;

struct PlotManagerGUI {
    PlotManagerGUI() = delete;

    static void sendTo(Player& player, std::shared_ptr<PlotHandle> handle);

private:
    static void handleEditName(
        Player&                     player,
        std::shared_ptr<PlotHandle> handle,
        std::optional<std::string>  lastInput = std::nullopt
    );

    static void handleEditSellStatus(Player& player, std::shared_ptr<PlotHandle> handle);
    static void sellPlotOrEditSellPrice(Player& player, std::shared_ptr<PlotHandle> handle, bool edit);

    static void showMembers(Player& player, std::shared_ptr<PlotHandle> handle);
    static void chooseAddType(Player& player, std::shared_ptr<PlotHandle> handle);
    static void addMemberFromOnline(Player& player, std::shared_ptr<PlotHandle> handle);
    static void addMemberFromOffline(Player& player, std::shared_ptr<PlotHandle> handle);
};

} // namespace plotx
