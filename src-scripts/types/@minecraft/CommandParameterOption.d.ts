declare module "@minecraft" {
    export const CommandParameterOption: CommandParameterOption;
    export type CommandParameterOption = NativeEnum<[
        "None",
        "EnumAutocompleteExpansion",
        "HasSemanticConstraint",
        "EnumAsChainedCommand"
    ]>;
}