declare module "@levilamina" {
    export class I18n {
        static getDefaultLocaleCode(): string;

        /**
         * 加载语言文件
         * @param fileOrDir 语言文件或目录
         * @throws {Error} 加载失败
         */
        static load(fileOrDir: string): void;

        static clear(): void;

        static set(localeCode: string, key: string, value: string): void;

        static get(key: string, localeCode: string): string;

        /**
         * 格式化字符串(辅助函数,底层为fmt)
         * @param fmt 格式化字符串
         * @param args 参数
         */
        static format(fmt: string, ...args: any[]): string;
    }
}
