declare module "minecraft" {
    export class UUID {
        constructor(uuid: string);

        asString(): string;

        static canParse(uuid: string): boolean;

        static fromString(uuid: string): UUID;
    }
}
