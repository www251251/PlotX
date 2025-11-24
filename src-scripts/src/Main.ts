import { registerCommand } from "./cmd/Command.js";

const self = ScriptMod.current();
self.onLoad(() => {
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
