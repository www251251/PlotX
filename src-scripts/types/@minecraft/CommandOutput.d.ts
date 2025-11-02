declare module "@minecraft" {

    export class CommandOutput implements InstanceClassHelper<CommandOutput> {
        $equals(other: CommandOutput): boolean;

        success(...args: any[]): void;

        error(...args: any[]): void;
    }

}