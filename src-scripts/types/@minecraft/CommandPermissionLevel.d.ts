declare module "@minecraft" {

    export type CommandPermissionLevel = NativeEnum<["Any",
        "GameDirectors",
        "Admin",
        "Host",
        "Owner",
        "Internal"]>;
    export const CommandPermissionLevel: CommandPermissionLevel;

}