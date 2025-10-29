declare module "@levilamina" {
    export class KeyValueDB implements InstanceClassHelper<KeyValueDB> {
        constructor(path: string);

        $equals(other: KeyValueDB): boolean;

        get(key: string): optional<string>;

        set(key: string, value: string): boolean;

        has(key: string): boolean;

        empty(): boolean;

        del(key: string): boolean;

        /**
         * Iterate over all keys and values in the database.
         * `this` inside the callback refers to the current KeyValueDB instance.
         *
         * @param fn Return `false` to stop iteration early,
         *           return any other value to continue.
         * @returns `true` if iteration finished normally (even if stopped early),
         *          `false` if a JS exception occurred.
         */
        iter(fn: (this: KeyValueDB, key: string, value: string) => boolean | any): boolean;

        /**
         * Close the database file.
         * @returns `true` if the file was closed successfully,
         *
         * @exception ReferenceError if the database is not open or has already been closed.
         * @note Once closed, the instance is no longer usable.
         */
        close(): boolean;
    }
}
