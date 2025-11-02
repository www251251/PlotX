declare module "@minecraft" {

    export const CommandOriginType: CommandOriginType;
    export type CommandOriginType = NativeEnum<[
        "Player",
        "CommandBlock",
        "MinecartCommandBlock",
        "DevConsole",
        "Test",
        "AutomationPlayer",
        "ClientAutomation",
        "DedicatedServer",
        "Entity",
        "Virtual",
        "GameArgument",
        "EntityServer",
        "Precompiled",
        "GameDirectorEntityServer",
        "Scripting",
        "ExecuteContext"
    ]>;

    export class CommandOrigin implements InstanceClassHelper<CommandOrigin> {
        $equals(other: CommandOrigin): boolean;

        getEntity(): Actor;

        getOriginType(): CommandOriginType;
    }

}