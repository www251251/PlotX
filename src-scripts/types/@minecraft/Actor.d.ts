declare module "@minecraft" {

    export class Actor implements InstanceClassHelper<Actor> {
        $equals(other: Actor): boolean;

        getTypeName(): string;

        getFeetPos(): Vec3;

        getFeetBlockPos(): BlockPos;

        isSimulatedPlayer(): boolean;

        teleport(pos: Vec3, dimid: number): void;

        despawn(): void;

        kill(): void;

        getDimensionId(): number;

        getPosition(): Vec3;

        isPlayer(): boolean;
    }

}