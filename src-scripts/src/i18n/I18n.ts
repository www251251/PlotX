import { I18n } from "@levilamina";

export function tryLoadI18n() {
    const mod = ScriptMod.current();
    try {
        const dir = mod.getLangDir();
        I18n.load(dir);
    } catch (e) {
        mod.getLogger().error("Failed to load i18n: ", e);
    }
}

function translate(key: string, localeCode: string, ...args: any[]) {
    const fmt = I18n.get(key, localeCode);
    return I18n.format(fmt, ...args);
}

export function tr(key: string, ...args: any[]) {
    return translate(key, I18n.getDefaultLocaleCode(), ...args);
}

export function trl(key: string, localeCode: string, ...args: any[]) {
    return translate(key, localeCode, ...args);
}
