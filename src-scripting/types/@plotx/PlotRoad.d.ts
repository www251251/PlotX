declare module "@plotx" {

    import {BlockPos} from "@minecraft";

    export class PlotRoad extends PlotAABB implements InstanceClassHelper<PlotRoad> {

        constructor();

        constructor(x: number, z: number);

        constructor(pos: BlockPos);

        x: number;
        z: number;
        isTransverse_: boolean;
        valid_: boolean;

        isTransverse(): boolean;

        isLongitudinal(): boolean;

        isValid(): boolean;

        toString(): string;

        removeNeighbourBorder(): void;
    }

}