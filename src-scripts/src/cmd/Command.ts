import {CommandRegistrar} from "@levilamina";
import {CommandOrigin, CommandOriginType, Player} from "@minecraft";


function isPlayer(origin: CommandOrigin) {
    // @ts-ignore
    return origin.getOriginType() == CommandOriginType.Player;
}


export function registerCommand() {
    let cmd = CommandRegistrar.getInstance().getOrCreateCommand("plotx");
    cmd.runtimeOverload().execute((origin, output, args) => {
        if (!isPlayer(origin)) {
            output.error(`This command can only be used by players.`);
            return;
        }
        const player = origin.getEntity() as Player;
        // TODO: implement
    });
}

