import { CommandHandle, CommandRegistrar } from "@levilamina";
import { CommandOriginType, Player } from "@minecraft";
import { PlotCoord } from "@plotx";
import { tr, trl } from "src/i18n/I18n";

export function registerCurrentPlotSubCommand(registrar: CommandRegistrar, handle: CommandHandle) {
    handle
        .runtimeOverload()
        .text("current")
        .execute((origin, output, args) => {
            if (origin.getOriginType() !== CommandOriginType.Player) {
                output.error(tr("Only players can use this command"));
                return;
            }

            const player = origin.getEntity() as Player;
            const coord = new PlotCoord(player.getPosition());
            if (!coord.isValid()) {
                output.error(trl("The player is not standing on a plot", player.getLocaleCode()));
                return;
            }

            // TODO: send PlotMain GUI
        });
}
