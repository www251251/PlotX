declare module "plotx" {
    export class logger {
        static trace(...args: any[]): void;

        static debug(...args: any[]): void;

        static info(...args: any[]): void;

        static warn(...args: any[]): void;

        static error(...args: any[]): void;

        static fatal(...args: any[]): void;
    }
}
