/// <reference path="@levilamina/index.d.ts" />

declare type ScriptModCallback = (mod: ScriptMod) => boolean;

declare class ScriptMod {
    onLoad(fn: ScriptModCallback): void;

    onEnable(fn: ScriptModCallback): void;

    onDisable(fn: ScriptModCallback): void;

    onUnload(fn: ScriptModCallback): void;

    getName(): string;

    getType(): string;

    getModDir(): string;

    getDataDir(): string;

    getConfigDir(): string;

    getLangDir(): string;

    isEnabled(): boolean;

    isDisabled(): boolean;

    getLogger(): import("@levilamina").Logger;

    /**
     * 获取当前 Mod 的实例
     */
    static current(): ScriptMod;
}



