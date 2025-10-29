declare module "@minecraft" {
    export class BlockPos implements InstanceClassHelper<BlockPos> {
        constructor(x: number, y: number, z: number);

        $equals(other: BlockPos): boolean;

        x: number;
        y: number;
        z: number;
    }
}
