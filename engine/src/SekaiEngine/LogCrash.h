/**
 * @file LogCrash.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Internal log ring buffer and fatal-fault crash dump
 * @version 0.1
 * @date 2026-09-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef _SEKAI_ENGINE_LOG_CRASH_H_
#define _SEKAI_ENGINE_LOG_CRASH_H_

#include <cstddef>

#include "SekaiEngine/Log.h"

namespace SekaiEngine
{
    /**
     * @brief Browser console method a level is routed to on Web builds
     * 
     */
    enum class ConsoleChannel
    {
        Log,
        Warn,
        Error
    };

    /**
     * @brief Map an engine level onto the Web console channel of matching severity
     * 
     */
    EXTENDAPI ConsoleChannel ChannelForLevel(LogLevel level);

    /**
     * @brief Bounded lock-free in-memory capture of the most recent log lines,
     *        plus the fatal-fault crash dump handlers (internal, not installed)
     * 
     * Append is lock-free and allocation-free: concurrent writers reserve a
     * slot with a compare-and-swap on the running byte counter and publish
     * with a release store, so no mutex or heap access happens on the logging
     * path. The capture is a fixed ring: when it is full the oldest bytes are
     * overwritten and dropped.
     * 
     * The crash handler is installed on the platform that supports it
     * (desktop). It writes crash-`<pid>`.log (in the working directory) from the
     * ring using only async-signal-safe operations, independently of whether a
     * file sink was requested.
     * 
     */
    class LogCrash
    {
    public:
        /**
         * @brief Append a newline-terminated line to the ring
         * 
         * @param bytes formatted line already carrying its trailing newline
         * @param len   length of bytes
         */
        EXTENDAPI static void Append(const char* bytes, size_t len);

        /**
         * @brief Copy the newest bytes of the ring out (for tests); the read
         *        side never allocates or locks either
         * 
         * @param out    caller-provided buffer
         * @param cap    size of out
         * @return size_t number of bytes copied (<= cap)
         */
        EXTENDAPI static size_t CopyLatest(char* out, size_t cap);
    };

    /**
     * @brief Install the crash handlers and resolve the crash-dump path once
     * 
     */
    void SekaiInstallCrashHandlers();
}

#endif //!_SEKAI_ENGINE_LOG_CRASH_H_