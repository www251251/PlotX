declare module "minecraft" {
    export type ModalFormCancelReason = NativeEnum<["UserClosed", "UserBusy"]>;
    export const ModalFormCancelReason: ModalFormCancelReason; // fix ts 18042
}
