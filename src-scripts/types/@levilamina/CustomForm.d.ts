import {Player} from "@minecraft";

declare module "@levilamina" {
    export type CustomFormElementResult = variant<
        [null | number | string] // std::monostate, uint64, double, std::string
    >;
    export type CustomFormResult<T extends Record<string, CustomFormElementResult>> = optional<T>;
    export type CustomFormCallback<T extends Record<string, CustomFormElementResult>> = (
        player: Player,
        result: CustomFormResult<T>,
        cancelReason: FormCancelReason
    ) => void;

    type _NoDuplicateKey_<T extends Record<string, any>, K extends string> = K extends keyof T ? never : K;

    export class CustomForm<T extends Record<string, CustomFormElementResult> = {}> implements InstanceClassHelper<CustomForm> {
        constructor();

        $equals(other: CustomForm<{}>): boolean;

        setTitle(title: string): this;

        setSubmitButton(text: string): this;

        appendHeader(header: string): this;

        appendLabel(label: string): this;

        appendDivider(): this;

        appendInput<K extends string>(
            key: _NoDuplicateKey_<T, K>,
            text: string,
            placeholder: string,
            defaultVal: string,
            tooltip: string
        ): CustomForm<T & { [P in K]: string }>; // this

        appendToggle<K extends string>(
            key: _NoDuplicateKey_<T, K>,
            text: string,
            defaultVal: boolean, // default: false
            tooltip: string
        ): CustomForm<T & { [P in K]: number }>; // this, true: 1, false: 0

        appendDropdown<K extends string>(
            key: _NoDuplicateKey_<T, K>,
            text: string,
            options: string[],
            defaultVal: number, // default: 0
            tooltip: string
        ): CustomForm<T & { [P in K]: string }>; // this

        appendSlider<K extends string>(
            key: _NoDuplicateKey_<T, K>,
            text: string,
            min: number,
            max: number,
            step: number, // default: 0.0
            defaultVal: number, // default: 0
            tooltip: string
        ): CustomForm<T & { [P in K]: number }>; // this

        appendStepSlider<K extends string>(
            key: _NoDuplicateKey_<T, K>,
            text: string,
            steps: string[],
            defaultVal: number, // default: 0
            tooltip: string
        ): CustomForm<T & { [P in K]: string }>; // this

        sendTo(player: Player, callback: CustomFormCallback<T>): this;

        sendUpdate(player: Player, callback: CustomFormCallback<T>): this;

        getFormData(): string;
    }
}
