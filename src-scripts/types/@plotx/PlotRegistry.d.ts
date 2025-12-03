declare module "@plotx" {
    import { UUID } from "@minecraft";

    export class PlotRegistry {
        isAdmin(uuid: UUID): boolean;

        addAdmin(uuid: UUID): void;

        removeAdmin(uuid: UUID): void;

        hasPlot(x: number, z: number): boolean;
        hasPlot(crood: PlotCoord): boolean;

        getPlot(x: number, z: number): optional<PlotHandle>;
        getPlot(crood: PlotCoord): optional<PlotHandle>;

        removePlot(x: number, z: number): boolean;
        removePlot(crood: PlotCoord): boolean;

        newPlot(x: number, z: number, owner: UUID): optional<PlotHandle>;
        newPlot(crood: PlotCoord, owner: UUID): optional<PlotHandle>;
    }
}
