import { UUID } from "@minecraft";

declare module "@levilamina" {
    export interface PlayerInfoEntry {
        uuid: UUID;
        name: string;
        xuid: string;
    }

    export class PlayerInfo {
        static fromUuid(uuid: UUID): PlayerInfoEntry | null;

        static fromName(name: string): PlayerInfoEntry | null;

        static fromXuid(xuid: string): PlayerInfoEntry | null;
    }
}
