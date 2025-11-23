declare module "@plotx" {

    import {UUID} from "@minecraft";

    export class PlotRegistry {
        isAdmin(uuid: UUID): boolean;

        addAdmin(uuid: UUID): void;

        removeAdmin(uuid: UUID): void;
    }

}