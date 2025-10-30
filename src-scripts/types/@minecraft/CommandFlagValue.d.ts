declare module "@minecraft" {
    export const CommandFlagValue: CommandFlagValue;
    export type CommandFlagValue = NativeEnum<[
        "None",
        "UsageTest",
        "HiddenFromCommandBlockOrigin",
        "HiddenFromPlayerOrigin",
        "HiddenFromAutomationOrigin",
        "SyncLocal",
        "ExecuteDisallowed",
        "TypeMessage",
        "NotCheat",
        "Async",
        "NoEditor",
        "Hidden",
        "Removed"]>;
}