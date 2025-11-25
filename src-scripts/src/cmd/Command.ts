import { CommandRegistrar } from "@levilamina";

import registerAdminSubcommand from "./subCommand/AdminSubCommand.js";

export function registerCommand() {
    const registrar = CommandRegistrar.getInstance();
    const cmd = registrar.getOrCreateCommand("plotx");

    registerAdminSubcommand(registrar, cmd);
}
