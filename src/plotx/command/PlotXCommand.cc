#include "PlotXCommand.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/gui/MainGUI.hpp"
#include "plotx/math/PlotCoord.hpp"

#include "ll/api/i18n/I18n.h"
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/command/Overload.h>
#include <ll/api/service/PlayerInfo.h>

#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOriginType.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPositionFloat.h"
#include "mc/server/commands/CommandSelector.h"
#include "plotx/core/PlotController.hpp"

#include <mc/world/actor/Actor.h>
#include <mc/world/actor/ActorType.h>
#include <mc/world/actor/player/Player.h>


namespace plotx {

using ll::i18n_literals::operator""_tr;
using ll::i18n_literals::operator""_trl;

#define GET_ENTITY_AND_CAST_PLAYER(_ORI) static_cast<Player&>(*_ORI.getEntity())

enum class AdminAction { Add, Remove };
struct AdminParam {
    AdminAction             action;
    std::string             name;
    CommandSelector<Player> players;

    void handle(CommandOutput& output, mce::UUID const& uuid, std::string const& displayName) const {
        auto registry = PlotX::getInstance().getPlotRegistry();
        switch (action) {
        case AdminAction::Add:
            if (registry->isAdmin(uuid)) {
                output.error("玩家 {} 已经是管理员"_tr(displayName));
            } else {
                registry->addAdmin(uuid);
                output.success("玩家 {} 已添加为管理员"_tr(displayName));
            }
            break;
        case AdminAction::Remove:
            if (registry->isAdmin(uuid)) {
                registry->removeAdmin(uuid);
                output.success("已移除玩家 '{}' 的管理员权限"_tr(displayName));
            } else {
                output.error("玩家 {} 不是管理员"_tr(displayName));
            }
            break;
        }
    }
};


void PlotXCommand::setup() {

    auto& handle = ll::command::CommandRegistrar::getInstance().getOrCreateCommand("plotx");

    // plotx admin <add|remove> <name>
    handle.overload<AdminParam>().text("admin").required("action").required("name").execute(
        [](CommandOrigin const& origin, CommandOutput& output, AdminParam const& param) {
            if (!ensureConsoleExecute(origin, output)) {
                return;
            }
            auto entry = ll::service::PlayerInfo::getInstance().fromName(param.name);
            if (!entry) {
                output.error("未找到玩家的UUID: {}"_tr(param.name));
                return;
            }
            param.handle(output, entry->uuid, entry->name);
        }
    );

    // plotx admins <add|remove> <@selector>
    handle.overload<AdminParam>().text("admins").required("action").required("players").execute(
        [](CommandOrigin const& origin, CommandOutput& output, AdminParam const& param) {
            if (!ensureConsoleExecute(origin, output)) {
                return;
            }
            auto players = param.players.results(origin).data;
            if (players->empty() || players->size() > 1) {
                output.error("请选择一个玩家进行添加"_tr());
                return;
            }
            auto player = players->at(0);
            param.handle(output, player->getUuid(), player->getRealName());
        }
    );

    // plotx switch_dimension
    handle.overload().text("switch_dimension").execute([](CommandOrigin const& origin, CommandOutput& output) {
        if (!ensurePlayerExecute(origin, output)) {
            return;
        }
        auto& player = GET_ENTITY_AND_CAST_PLAYER(origin);
        PlotX::getInstance().getController()->switchPlayerDimension(player);
    });

    // plotx current
    handle.overload().text("current").execute([](CommandOrigin const& origin, CommandOutput& output) {
        if (!ensurePlayerExecute(origin, output)) {
            return;
        }
        auto& player = GET_ENTITY_AND_CAST_PLAYER(origin);
        if (!ensurePlayerInPlotDimension(player, output)) {
            return;
        }
        PlotX::getInstance().getController()->sendPlayerCurrentPlot(player);
    });

    // plotx
    handle.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        if (!ensurePlayerExecute(origin, output)) {
            return;
        }
        auto& player = GET_ENTITY_AND_CAST_PLAYER(origin);
        MainGUI::sendTo(player);
    });

    // plotx teleport unowned_plot
    handle.overload()
        .text("teleport")
        .text("unowned_plot")
        .execute([](CommandOrigin const& origin, CommandOutput& output) {
            if (!ensurePlayerExecute(origin, output)) {
                return;
            }
            auto& player = GET_ENTITY_AND_CAST_PLAYER(origin);
            if (!ensurePlayerInPlotDimension(player, output)) {
                return;
            }
            PlotX::getInstance().getController()->teleportUnownedPlot(player);
        });
}
bool PlotXCommand::ensureConsoleExecute(CommandOrigin const& origin, CommandOutput& output) {
    if (origin.getOriginType() != CommandOriginType::DedicatedServer) {
        output.error("此命令仅允许在控制台执行"_tr());
        return false;
    }
    return true;
}
bool PlotXCommand::ensurePlayerExecute(CommandOrigin const& origin, CommandOutput& output) {
    if (origin.getOriginType() != CommandOriginType::Player) {
        output.error("此命令仅允许玩家执行"_tr());
        return false;
    }
    return true;
}
bool PlotXCommand::ensurePlayerInPlotDimension(Player& player, CommandOutput& output) {
    if (player.getDimensionId() != PlotX::getDimensionId()) {
        output.error("此命令仅允许在地皮维度执行"_tr());
        return false;
    }
    return true;
}


} // namespace plotx