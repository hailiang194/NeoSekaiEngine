/**
 * @file Log.cpp
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Engine logging facility implementation
 * @version 0.1
 * @date 2026-09-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "SekaiEngine/Log.h"
#include "SekaiEngine/LogCrash.h"

#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>

#if !defined(PLATFORM_WEB)
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Record.h>
#endif

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace SekaiEngine
{
    namespace
    {
        constexpr int kMaxFileBytes = 1024 * 1024;
        constexpr int kMaxFiles = 5;
        constexpr size_t kMessageCapacity = 1024;
        constexpr size_t kLineCapacity = 2048;

        const char* kLevelNames[] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };

        std::atomic<LogLevel> g_minLevel{LogLevel::Info};
        std::atomic<unsigned> g_ordinal{0};

        // Renders the wall-clock time of day as "HH:MM:SS.mmm". Runs only on the
        // normal formatting path (never inside the crash handler), so the ring
        // continues to receive fully pre-formatted bytes.
        int FormatTimestamp(char* buf, size_t n)
        {
            const auto now = std::chrono::system_clock::now();
            const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                now.time_since_epoch())
                            % 1000;
            const std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm tm = {};
#if defined(_WIN32)
            localtime_s(&tm, &t);
#else
            localtime_r(&t, &tm);
#endif
            return snprintf(buf, n, "%02d:%02d:%02d.%03d",
                            tm.tm_hour, tm.tm_min, tm.tm_sec, static_cast<int>(ms.count()));
        }

        // Serializes sink dispatch so every line reaches the console and the
        // optional file in one uninterruptible step with no torn output.
        std::mutex g_sinkMutex;

#if defined(PLATFORM_WEB)
        void DispatchToSink(LogLevel level, const char* line)
        {
            switch (ChannelForLevel(level))
            {
                case ConsoleChannel::Error:
                    emscripten_console_error(line);
                    break;
                case ConsoleChannel::Warn:
                    emscripten_console_warn(line);
                    break;
                case ConsoleChannel::Log:
                default:
                    emscripten_console_log(line);
                    break;
            }
        }
#else
        using MessageFormatter = plog::MessageOnlyFormatter;

        plog::Severity ToPlogSeverity(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::Trace:   return plog::verbose;
                case LogLevel::Debug:   return plog::debug;
                case LogLevel::Warning: return plog::warning;
                case LogLevel::Error:   return plog::error;
                case LogLevel::Fatal:   return plog::fatal;
                case LogLevel::Info:
                default:                return plog::info;
            }
        }

        plog::ConsoleAppender<MessageFormatter> g_console;

        // Disabled until Log::SetFile is called: an empty file name is held so
        // that no file is ever created unless a file sink was requested.
        plog::RollingFileAppender<MessageFormatter> g_file("", kMaxFileBytes, kMaxFiles);
        bool g_fileEnabled = false;

        void DispatchToSink(LogLevel level, const char* line)
        {
            std::lock_guard<std::mutex> lock(g_sinkMutex);

            plog::Record record(ToPlogSeverity(level), "", 0, "", nullptr, 0);
            record << line;

            g_console.write(record);
            if (g_fileEnabled)
            {
                g_file.write(record);
            }
        }
#endif

        void SetFileIfRequested(const char* path)
        {
            if (nullptr == path || '\0' == path[0])
            {
                return;
            }

#if !defined(PLATFORM_WEB)
            {
                std::lock_guard<std::mutex> lock(g_sinkMutex);
                g_file.setFileName(path);
                g_file.setMaxFileSize(kMaxFileBytes);
                g_file.setMaxFiles(kMaxFiles);
                g_fileEnabled = true;
            }
#else
            // Web logs go to the browser console only; no file is written.
            (void)path;
#endif
        }

        // Static init: install the crash handlers and honor the environment
        // variable before any call to Write. Running at load time (before
        // main) keeps the Write path free of first-call initialization.
        struct LogBootstrap
        {
            LogBootstrap()
            {
                SekaiInstallCrashHandlers();
                SetFileIfRequested(std::getenv("SEKAI_LOG_FILE"));
            }
        };

        LogBootstrap g_logBootstrap;
    } // namespace

    void Log::Write(LogLevel level, const char* fmt, ...)
    {
        if (static_cast<int>(level) < static_cast<int>(g_minLevel.load(std::memory_order_relaxed)))
        {
            return;
        }

        char message[kMessageCapacity];
        va_list args;
        va_start(args, fmt);
        vsnprintf(message, sizeof(message), fmt, args);
        va_end(args);

        const unsigned ordinal = g_ordinal.fetch_add(1, std::memory_order_relaxed);
        const char* levelName = kLevelNames[static_cast<size_t>(level)];

        char timestamp[32];
        FormatTimestamp(timestamp, sizeof(timestamp));

        char raw[kLineCapacity];
        const int n = snprintf(raw, sizeof(raw), "[%s] #%u %s %s",
                               levelName, ordinal, timestamp, message);
        if (n < 0)
        {
            return;
        }
        const size_t rawLen = (static_cast<size_t>(n) < sizeof(raw)) ? static_cast<size_t>(n) : sizeof(raw) - 1;

        LogCrash::Append(raw, rawLen);
        LogCrash::Append("\n", 1);

        DispatchToSink(level, raw);
    }

    void Log::SetLevel(LogLevel level)
    {
        g_minLevel.store(level, std::memory_order_relaxed);
    }

    void Log::SetFile(const char* path)
    {
        SetFileIfRequested(path);
    }

    ConsoleChannel ChannelForLevel(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Fatal:
            case LogLevel::Error:
                return ConsoleChannel::Error;
            case LogLevel::Warning:
                return ConsoleChannel::Warn;
            case LogLevel::Trace:
            case LogLevel::Debug:
            case LogLevel::Info:
            default:
                return ConsoleChannel::Log;
        }
    }
} // namespace SekaiEngine