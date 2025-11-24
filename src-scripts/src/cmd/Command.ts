import { CommandHandle, CommandParamKind, CommandRegistrar } from "@levilamina";
import { CommandOriginType } from "@minecraft";
import { ArgType } from "../utils/TypeUtils";

function registerAdminSubcommand(registrar: CommandRegistrar, cmd: CommandHandle) {
    const kPlotXAdminEnumKey = "PlotXAdminActions";

    type RuntimeEnumType = ArgType<typeof CommandRegistrar.prototype.tryRegisterRuntimeEnum, 1>;

    const kPlotXAdminEnumValues: RuntimeEnumType = [
        ["add", 0],
        ["remove", 1],
    ];

    if (!registrar.hasEnum(kPlotXAdminEnumKey)) {
        registrar.tryRegisterRuntimeEnum(kPlotXAdminEnumKey, kPlotXAdminEnumValues);
    }

    cmd.runtimeOverload()
        .text("admin")
        .required("action", CommandParamKind.Enum, kPlotXAdminEnumKey)
        .execute((origin, output, args) => {
            if (origin.getOriginType() != CommandOriginType.DedicatedServer) {
                output.error("Only server can execute this command");
                return;
            }
            const { action } = args;
            ScriptMod.current().getLogger().info("Admin action: ", action);
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
