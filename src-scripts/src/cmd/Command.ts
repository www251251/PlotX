import { CommandHandle, CommandParamKind, CommandRegistrar } from "@levilamina";
import { CommandOriginType } from "@minecraft";
import { ArgType } from "../utils/TypeUtils";

function registerAdminSubcommand(registrar: CommandRegistrar, cmd: CommandHandle) {
    const kPlotXAdminEnumKey = "PlotXAdminActions";
    const kPlotXAdminEnumValues = ["add", "remove"] as const;

    if (!registrar.hasSoftEnum(kPlotXAdminEnumKey)) {
        type RuntimeEnumType = ArgType<typeof CommandRegistrar.prototype.tryRegisterSoftEnum, 1>;
        registrar.tryRegisterSoftEnum(kPlotXAdminEnumKey, kPlotXAdminEnumValues as unknown as RuntimeEnumType);
    }

    cmd.runtimeOverload()
        .text("admin")
        .required("action", CommandParamKind.SoftEnum, kPlotXAdminEnumKey)
        .execute((origin, output, args) => {
            if (origin.getOriginType() != CommandOriginType.DedicatedServer) {
                output.error("Only server can execute this command");
                return;
            }
            ScriptMod.current().getLogger().info("Executing plotx admin command", JSON.stringify(args));
            const action = args.action as (typeof kPlotXAdminEnumValues)[number];
            switch (action) {
                case "add":
                    // TODO: add admin
                    break;
                case "remove":
                    // TODO: remove admin
                    break;
                default:
                    throw new Error("Unknown action");
            }
        });
}

export function registerCommand() {
    const registrar = CommandRegistrar.getInstance();
    const cmd = registrar.getOrCreateCommand("plotx");

    registerAdminSubcommand(registrar, cmd);
}
