#pragma once

class Player;

namespace plotx {

struct MainGUI {
    MainGUI() = delete;

    static void sendTo(Player& player);
};

} // namespace plotx
