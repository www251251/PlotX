import { CommandHandle, CommandParamKind, CommandRegistrar } from "@levilamina";
import { CommandOriginType, UUID } from "@minecraft";

import { ArgType } from "../../utils/TypeUtils";

const kPlotXAdminEnumKey = "PlotXAdminActions";
const kPlotXAdminEnumValues = [
    ["add", 0],
    ["remove", 1],
] as const;

type AdminActionEnumNameType = (typeof kPlotXAdminEnumValues)[number][0];

function handleAction(type: AdminActionEnumNameType, target: UUID) {
    switch (type) {
        case "add":
        // TODO: bind PlotRegistry API, add player to admin list
        case "remove":
        default:
            throw new Error(`Unknown action ${type}`);
    }
}

export default function registerAdminSubcommand(registrar: CommandRegistrar, cmd: CommandHandle) {
    if (!registrar.hasEnum(kPlotXAdminEnumKey)) {
        type RuntimeEnumType = ArgType<typeof CommandRegistrar.prototype.tryRegisterRuntimeEnum, 1>;
        registrar.tryRegisterRuntimeEnum(kPlotXAdminEnumKey, kPlotXAdminEnumValues as unknown as RuntimeEnumType);
    }

    // TODO: bind i18n API, change all output to i18n
    // plotx admin <add|remove> <realName: string>
    cmd.runtimeOverload()
        .text("admin")
        .required("action", CommandParamKind.Enum, kPlotXAdminEnumKey)
        .required("realName", CommandParamKind.String)
        .execute((origin, output, args) => {
            if (origin.getOriginType() != CommandOriginType.DedicatedServer) {
                output.error("Only server can execute this command");
                return;
            }
            const { realName, action } = args;
            if (!realName) {
                output.error("No real name specified");
                return;
            }
            // TODO: bind PlayerInfo API, query player uuid by real name
            // handleAction(action as AdminEnumType, realName);
        });

    // plotx admins <add|remove> <players: target>
    cmd.runtimeOverload()
        .text("admins")
        .required("action", CommandParamKind.Enum, kPlotXAdminEnumKey)
        .required("players", CommandParamKind.Player)
        .execute((origin, output, args) => {
            if (origin.getOriginType() !== CommandOriginType.DedicatedServer) {
                output.error("Only server can execute this command");
                return;
            }
            const { players, action } = args;
            switch (players.length) {
                case 0:
                    output.error("No players specified");
                    return;
                case 1:
                    handleAction(action as AdminActionEnumNameType, players[0].uuid);
                    break;
                default:
                    output.error("Only one player can be specified");
            }
        });
}
