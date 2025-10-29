declare module "@minecraft" {
    export class Vec3 implements InstanceClassHelper<Vec3> {
        $equals(other: Vec3): boolean;

        constructor(x: number, y: number, z: number);

        x: number;
        y: number;
        z: number;

        isNan(): boolean;

        isNear(rhs: Vec3, e: number): boolean;

        toJsonString(): string;

        toString(): string;
    }
}
