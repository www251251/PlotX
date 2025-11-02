declare module "@levilamina" {

    import {
        BlockPos,
        CommandFlagValue,
        CommandOrigin, CommandOutput,
        CommandParameterOption,
        CommandPermissionLevel,
        Player,
        Vec3
    } from "@minecraft";

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

        runtimeOverload(): RuntimeOverload<Params>;
    }

    export enum CommandParamKind {
        // $name: string;
        Int,// 0
        Bool,// 1
        Float,// 2
        Dimension,// 3
        String,// 4
        Enum,// 5
        SoftEnum,// 6
        Actor,// 7
        Player,// 8
        BlockPos,// 9
        Vec3,// 10
        RawText,// 11
        Message,// 12
        JsonValue,// 13
        Item,// 14
        BlockName,// 15
        BlockState,// 16
        Effect,// 17
        ActorType,// 18
        Command,// 19
        RelativeFloat,// 20
        IntegerRange,// 21
        FilePath,// 22
        WildcardInt,// 23
        WildcardActor,// 24
        Count,// 25
    }

    type KindToType<K extends CommandParamKind> =
        K extends CommandParamKind.Int ? number :
            K extends CommandParamKind.Float ? number :
                K extends CommandParamKind.Enum ? string :
                    K extends CommandParamKind.SoftEnum ? string :
                        K extends CommandParamKind.String ? string :
                            K extends CommandParamKind.RawText ? string :
                                K extends CommandParamKind.Bool ? boolean :
                                    K extends CommandParamKind.Player ? Player[] :
                                        K extends CommandParamKind.BlockPos ? BlockPos :
                                            K extends CommandParamKind.Vec3 ? Vec3 : never;

    type SupportedKind =
        CommandParamKind.Int |
        CommandParamKind.Float |
        CommandParamKind.Enum |
        CommandParamKind.SoftEnum |
        CommandParamKind.String |
        CommandParamKind.RawText |
        CommandParamKind.Bool |
        CommandParamKind.Player |
        CommandParamKind.BlockPos |
        CommandParamKind.Vec3;

    type Params = Record<string, any>;

    export class RuntimeOverload<T extends Params> implements InstanceClassHelper<RuntimeOverload<T>> {
        $equals<H extends Params>(other: RuntimeOverload<H>): boolean;

        optional<N extends string, K extends CommandParamKind>(
            name: N,
            kind: K extends SupportedKind ? K : never
        ): RuntimeOverload<T & { [P in N]?: KindToType<K> }>;

        required<N extends string, K extends CommandParamKind>(
            name: N,
            kind: K extends SupportedKind ? K : never
        ): RuntimeOverload<T & { [P in N]: KindToType<K> }>;

        optional<N extends string>(
            name: N,
            enumKind: CommandParamKind.Enum | CommandParamKind.SoftEnum,
            enumName: string
        ): RuntimeOverload<T & { [P in N]?: string }>;

        required<N extends string>(
            name: N,
            enumKind: CommandParamKind.Enum | CommandParamKind.SoftEnum,
            enumName: string
        ): RuntimeOverload<T & { [P in N]: string }>;

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
            fn: (origin: CommandOrigin, output: CommandOutput, args: T) => void
        ): this;
    }
}