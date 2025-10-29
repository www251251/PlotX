declare module "@minecraft" {
    export class UUID implements InstanceClassHelper<UUID> {
        $equals(other: UUID): boolean;

        constructor(uuid: string);

        asString(): string;

        static canParse(uuid: string): boolean;

        static fromString(uuid: string): UUID;
    }
}
