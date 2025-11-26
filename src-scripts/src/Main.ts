import { tryLoadI18n } from "i18n/I18n.js";
import { registerCommand } from "./cmd/Command.js";

const self = ScriptMod.current();
self.onLoad(() => {
    tryLoadI18n();

    return true;
});

self.onEnable(() => {
    registerCommand();
    return true;
});

self.onDisable(() => {
    return true;
});

self.onUnload(() => {
    return true;
});
