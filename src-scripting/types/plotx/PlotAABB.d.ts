import { BlockPos } from "minecraft";

declare module "plotx" {
    export type PlotAABBForEachCallBack = (pos: BlockPos) => boolean;

    export class PlotAABB implements InstanceClassHelper<PlotAABB> {
        constructor(min: BlockPos, max: BlockPos);
        $equals(other: PlotAABB): boolean;

        get min(): BlockPos;
        set min(min: BlockPos);

        get max(): BlockPos;
        set max(max: BlockPos);

        toString(): string;

        getMin(): BlockPos; // copy
        getMax(): BlockPos;

        isOnEdge(pos: BlockPos): boolean;

        contains(pos: BlockPos): boolean;

        getEdges(y: number): BlockPos[];

        getVertices(closure: boolean): BlockPos[];

        forEach(fn: PlotAABBForEachCallBack): void;

        forEachLayer(y: number, fn: PlotAABBForEachCallBack): void;

        forEachEdge(fn: PlotAABBForEachCallBack): void;

        forEachEdgeLayer(y: number, fn: PlotAABBForEachCallBack): void;
    }
}
