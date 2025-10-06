import { Player } from "minecraft";

declare module "levilamina" {
    export type ModalFormSelectedButton = NativeEnum<["Upper", "Lower"]>; // Upper: true, Lower: false
    export const ModalFormSelectedButton: ModalFormSelectedButton; // fix ts 18042
    export type ModalFormResult = optional<ModalFormSelectedButton>;
    export type ModalFormCallback = (player: Player, result: ModalFormResult, cancelReason: FormCancelReason) => void;

    export class ModalForm {
        constructor();

        setTitle(title: string): this;

        setContent(content: string): this;

        setUpperButton(text: string): this;

        setLowerButton(text: string): this;

        sendTo(player: Player, callback: ModalFormCallback): this;

        sendUpdate(player: Player, callback: ModalFormCallback): this;
    }
}
