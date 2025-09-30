// shims.d.ts
declare namespace emakefun {
    /**
     * Simultaneously search for multiple target strings in a serial data stream.
     * @param targets The target string array to be searched for.
     * @param timeout_ms Timeout for waiting for response (milliseconds).
     * @returns Find the index of the target string in the array, return -1 if not found.
     */
    //% shim=emakefun::multiFindUtil
    //% blockHidden=true
    function multiFindUtil(targets: string[], targets_size: int32, timeout_ms: int32): number;

    /**
     * Search for a single target string in the serial data stream.
     * @param target The target string to be searched for.
     * @param timeout_ms Timeout for waiting for response (milliseconds).
     * @returns Whether the target string has been found, true: found, false: not found.
     */
    //% shim=emakefun::singleFindUtil
    //% blockHidden=true
    function singleFindUtil(target: string, timeout_ms: int32): boolean;

    /**
     * Skip the next character and return true if it matches the target character.
     * @param target Target characters.
     * @param timeout_ms Timeout for waiting for response (milliseconds).
     * @returns Match and skip target characters, true: successful, false: failed.
     */
    //% shim=emakefun::skipNext
    //% blockHidden=true
    function skipNext(target: string, timeout_ms: int32): boolean;

    /**
     * Parse integers from serial data streams.
     * @param timeout_ms Timeout for waiting for response (milliseconds).
     * @returns The parsed integer value returns -1 upon timeout or failure.
     */
    //% shim=emakefun::parseNumber
    //% blockHidden=true
    function parseNumber(timeout_ms: int32): number;

    /**
     * Read from serial until delimiter is found.
     * @param delimiter The delimiter character.
     * @param timeout_ms Timeout for waiting for response (milliseconds).
     * @returns The read string until delimiter, or null if timeout.
     */
    //% shim=emakefun::readUntil
    //% blockHidden=true
    function readUntil(delimiter: string, timeout_ms: int32): string;

    /**
     * Clear the serial receive buffer.
     * @param timeout_ms Timeout for clearing operation (milliseconds).
     */
    //% shim=emakefun::emptyRx
    //% blockHidden=true
    function emptyRx(timeout_ms: int32): void;
}