/**
 * @file Log.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Engine-wide logging facility
 * @version 0.1
 * @date 2026-09-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef _SEKAI_ENGINE_LOG_H_
#define _SEKAI_ENGINE_LOG_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    /**
     * @brief Log severity level, ordered from least to most severe
     * 
     */
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    /**
     * @brief Entry point of the engine logging facility
     * 
     * Messages are printed to the console by default. On desktop platforms a
     * rotating file sink can be added with SetFile() (or the SEKAI_LOG_FILE
     * environment variable); the most recent messages are also captured in a
     * bounded in-memory ring that a fatal-fault handler dumps to
     * crash-`<pid>`.log.
     * 
     */
    class EXTENDAPI Log
    {
    public:
        /**
         * @brief Write a formatted message at the given level, if it passes the
         *        configured threshold (see SetLevel)
         * 
         * Each line is rendered as the level, a wall-clock timestamp and the
         * message, e.g. `[INFO] #0 12:34:56.789 <message>`; the source file is
         * not part of the output.
         * 
         * @param level severity of the message
         * @param fmt   printf-style format string
         */
        static void Write(LogLevel level, const char* fmt, ...);

        /**
         * @brief Set the minimum severity that reaches any sink
         * 
         * @param level highest level that is suppressed; anything more severe
         *              is printed to console, logged to file and captured in the ring
         */
        static void SetLevel(LogLevel level);

        /**
         * @brief Enable the rotating desktop file sink for the given path
         * 
         * A bounded file is created and appended to, rotating when it grows too
         * large. No file is ever created unless this is called. On Web this is
         * a no-op: logs go to the browser console only.
         * 
         * @param path absolute or relative path of the log file
         */
        static void SetFile(const char* path);
    };
}

/**
 * @brief Write a formatted message at the Trace level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_TRACE(...)     ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Trace,   __VA_ARGS__)
/**
 * @brief Write a formatted message at the Debug level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_DEBUG(...)     ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Debug,   __VA_ARGS__)
/**
 * @brief Write a formatted message at the Info level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_INFO(...)      ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Info,    __VA_ARGS__)
/**
 * @brief Write a formatted message at the Warning level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_WARNING(...)   ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Warning, __VA_ARGS__)
/**
 * @brief Write a formatted message at the Error level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_ERROR(...)     ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Error,   __VA_ARGS__)
/**
 * @brief Write a formatted message at the Fatal level
 *
 * @param ... a printf-style format string followed by its arguments
 */
#define SEKAI_FATAL(...)     ::SekaiEngine::Log::Write(::SekaiEngine::LogLevel::Fatal,   __VA_ARGS__)

#endif //!_SEKAI_ENGINE_LOG_H_