declare module "@levilamina" {

    import {CommandFlagValue, CommandParameterOption, CommandPermissionLevel} from "@minecraft";

    export class CommandRegistrar implements InstanceClassHelper<CommandRegistrar> {
        $equals(other: CommandRegistrar): boolean;

        static getInstance(): CommandRegistrar;

        getOrCreateCommand(
            name: string,
            description?: string,
            requirement?: CommandPermissionLevel,
            flag?: CommandFlagValue
        ): CommandHandle;
    }

    export class CommandHandle implements InstanceClassHelper<CommandHandle> {
        $equals(other: CommandHandle): boolean;

        runtimeOverload(): RuntimeOverload;
    }

    // TODO: add types for CommandOrigin and CommandOutput
    // export type RuntimeCommandCallback = (origin: CommandOrigin, output: CommandOutput) => void;
    export type RuntimeCommandCallback = () => void;

    export class RuntimeOverload implements InstanceClassHelper<RuntimeOverload> {
        $equals(other: RuntimeOverload): boolean;

        optional(
            name: string,
            kind: CommandParamKind
        ): this;

        required(
            name: string,
            kind: CommandParamKind
        ): this;

        optional(
            name: string,
            enumKind: CommandParamKind,
            enumName: string
        ): this;

        required(
            name: string,
            enumKind: CommandParamKind,
            enumName: string
        ): this;

        text(
            text: string
        ): this;

        postfix(
            postfix: string
        ): this;

        option(
            option: CommandParameterOption
        ): this;

        deoption(
            option: CommandParameterOption
        ): this;

        execute(
            fn: RuntimeCommandCallback
        ): this;
    }

    export const CommandParamKind: CommandParamKind;
    export type CommandParamKind = NativeEnum<[
        "Int",
        "Bool",
        "Float",
        "Dimension",
        "String",
        "Enum",
        "SoftEnum",
        "Actor",
        "Player",
        "BlockPos",
        "Vec3",
        "RawText",
        "Message",
        "JsonValue",
        "Item",
        "BlockName",
        "BlockState",
        "Effect",
        "ActorType",
        "Command",
        "RelativeFloat",
        "IntegerRange",
        "FilePath",
        "WildcardInt",
        "WildcardActor",
    ]>

}