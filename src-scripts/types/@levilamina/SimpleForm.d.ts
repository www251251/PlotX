import {Player} from "@minecraft";

declare module "@levilamina" {
    export type SimpleFormButtonCallback = (player: Player) => void;
    export type SimpleFormCallback = (player: Player, index: number, cancelReason: FormCancelReason) => void;

    export class SimpleForm implements InstanceClassHelper<SimpleForm> {
        constructor();

        $equals(other: SimpleForm): boolean;

        setTitle(title: string): this;

        setContent(content: string): this;

        appendHeader(header: string): this;

        appendLabel(label: string): this;

        appendDivider(): this;

        /**
         * Append a button to the form
         * @param  text      The text of the button
         * @param  imageData The image data of the button
         * @param  imageType The type of the image (url or path)
         * @param  callback  The callback of the button
         * @return SimpleForm&  *this
         * @note   If the `callback` parameter of sendTo() is set, the callback of the button will be ignored
         */
        appendButton(
            text: string,
            imageData: string,
            imageType: "url" | "path",
            callback: SimpleFormButtonCallback
        ): this;

        /**
         * Append a button to the form
         * @param  text      The text of the button
         * @param  callback  The callback of the button
         * @return SimpleForm&  *this
         * @note   If the `callback` parameter of sendTo() is set, the callback of the button will be ignored
         */
        appendButton(text: string, callback: SimpleFormButtonCallback): this;

        /**
         * Send the form to the player
         * @param  player    The player to receive the form
         * @param  callback  The callback of the form(if set, the callback of the button will be ignored)
         * @return SimpleForm&  *this
         * @note   If `callback` is set, the callbacks of each buttons will be ignored
         */
        sendTo(player: Player, callback: SimpleFormCallback): this;

        sendUpdate(player: Player, callback: SimpleFormCallback): this;
    }
}
