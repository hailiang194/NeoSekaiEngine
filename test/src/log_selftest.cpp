/**
 * @file log_selftest.cpp
 * @brief Framework-free (no Googletest) self-check for the logging facility
 *
 * Exercises the spec scenarios of specs/logging/spec.md: default console +
 * opt-in file, level gate, line format (level + timestamp, no source file),
 * ordinal, ring capture, concurrent writes, the crash dump, and the Web /
 * raylib level mappings. Exit code 0 means every check passed.
 *
 * Order note: NoFileUntilRequested must run first, before any SetFile call,
 * because the file sink stays globally enabled once requested.
 *
 * @copyright Copyright (c) 2024
 */
#include "SekaiEngine/Log.h"
#include "SekaiEngine/LogCrash.h"
#include "wrapper/raylib/Log.h"

#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Record.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace
{
    size_t g_checks = 0;
    size_t g_failures = 0;

#define CHECK(cond)                                                                      \
    do                                                                                   \
    {                                                                                    \
        ++g_checks;                                                                      \
        if (!(cond))                                                                     \
        {                                                                                \
            ++g_failures;                                                                \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);         \
        }                                                                                \
    } while (0)

    std::string UniquePath(const std::string& suffix)
    {
        static std::atomic<unsigned> seq{0};
        std::ostringstream os;
        os << "sekai_log_" << seq.fetch_add(1) << "_" << suffix;
        return os.str();
    }

    bool Exists(const std::string& path)
    {
        std::ifstream f(path.c_str());
        return f.good();
    }

    std::string ReadFile(const std::string& path)
    {
        std::ifstream f(path.c_str(), std::ios::binary);
        std::ostringstream os;
        os << f.rdbuf();
        return os.str();
    }

    std::string CrashPathFor(unsigned long pid)
    {
        return "crash-" + std::to_string(pid) + ".log";
    }

    void NoFileUntilRequested()
    {
        const std::string path = UniquePath("no_file.log");
        std::remove(path.c_str());
        SEKAI_INFO("this message goes to the console only, marker=NO_FILE_PROBE");
        CHECK(!Exists(path));
        std::remove(path.c_str());
    }

    void LevelGateSuppressesAndRoutes()
    {
        const std::string path = UniquePath("gate.log");
        std::remove(path.c_str());
        SekaiEngine::Log::SetFile(path.c_str());

        SekaiEngine::Log::SetLevel(SekaiEngine::LogLevel::Warning);
        SEKAI_INFO("suppressed under a warning gate, marker=GATE_SUPPRESSED");
        SEKAI_ERROR("allowed under a warning gate, marker=GATE_ALLOWED");
        SekaiEngine::Log::SetLevel(SekaiEngine::LogLevel::Info);

        const std::string content = ReadFile(path);
        CHECK(content.find("GATE_SUPPRESSED") == std::string::npos);
        CHECK(content.find("GATE_ALLOWED") != std::string::npos);
        std::remove(path.c_str());
    }

    void FormattedLineAndOrdinal()
    {
        const std::string path = UniquePath("format.log");
        std::remove(path.c_str());
        SekaiEngine::Log::SetFile(path.c_str());

        SEKAI_INFO("first marker=%d", 42);
        SEKAI_WARNING("second marker=%s", "wow");

        const std::string content = ReadFile(path);
        CHECK(content.find("first marker=42") != std::string::npos);
        CHECK(content.find("second marker=wow") != std::string::npos);

        // every line carries its level and a HH:MM:SS.mmm timestamp ...
        CHECK(std::regex_search(content,
                                std::regex("\\[INFO\\] #[0-9]+ [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} first marker=42")));
        CHECK(std::regex_search(content,
                                std::regex("\\[WARNING\\] #[0-9]+ [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} second marker=wow")));

        // ... and no source file path leaks into the output
        CHECK(content.find("log_selftest.cpp") == std::string::npos);
        CHECK(content.find('/') == std::string::npos);

        // ordinal of the second message is strictly greater than the first
        std::smatch firstOrdinal;
        std::smatch secondOrdinal;
        CHECK(std::regex_search(content, firstOrdinal,
                                std::regex("\\[INFO\\] #([0-9]+) [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} first marker=42")));
        CHECK(std::regex_search(content, secondOrdinal,
                                std::regex("\\[WARNING\\] #([0-9]+) [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} second marker=wow")));
        CHECK(std::stol(secondOrdinal[1].str()) > std::stol(firstOrdinal[1].str()));
        std::remove(path.c_str());
    }

    void ConcurrentWritesProduceCompleteLines()
    {
        const std::string path = UniquePath("threads.log");
        std::remove(path.c_str());
        SekaiEngine::Log::SetFile(path.c_str());

        constexpr size_t kMessages = 400;
        std::thread first([kMessages]()
        {
            for (size_t i = 0; i < kMessages; ++i)
            {
                SEKAI_INFO("HEAD_%zu", i);
            }
        });
        std::thread second([kMessages]()
        {
            for (size_t i = 0; i < kMessages; ++i)
            {
                SEKAI_INFO("TAIL_%zu", i);
            }
        });
        first.join();
        second.join();

        const std::string content = ReadFile(path);

        // one physical line per message means no torn or interleaved writes
        const size_t lines = static_cast<size_t>(std::count(content.begin(), content.end(), '\n'));
        CHECK(lines == 2 * kMessages);

        // every marker appears in the file exactly once, fully intact
        size_t headTotal = 0;
        size_t tailTotal = 0;
        size_t pos = 0;
        while ((pos = content.find("HEAD_", pos)) != std::string::npos)
        {
            ++headTotal;
            ++pos;
        }
        pos = 0;
        while ((pos = content.find("TAIL_", pos)) != std::string::npos)
        {
            ++tailTotal;
            ++pos;
        }
        CHECK(headTotal + tailTotal == 2 * kMessages);
        std::remove(path.c_str());
    }

    void RingKeepsNewestWhenFull()
    {
        char out[4096];

        // order preserved, oldest first: the newest bytes are exactly what we
        // appended last
        SekaiEngine::LogCrash::Append("AAA", 3);
        SekaiEngine::LogCrash::Append("BBB", 3);
        const size_t got = SekaiEngine::LogCrash::CopyLatest(out, sizeof(out));
        CHECK(got >= 6);
        CHECK(std::string(out + got - 6, 6) == std::string("AAABBB"));

        // overflow: with ~300 KiB of 1 KiB lines, only the tail survives
        const std::string line = std::string(1012, 'x') + "END_MARKER8899";
        for (size_t i = 0; i < 300; ++i)
        {
            SekaiEngine::LogCrash::Append(line.c_str(), line.size());
        }

        const size_t n = SekaiEngine::LogCrash::CopyLatest(out, sizeof(out));
        CHECK(n > 0);
        CHECK(std::string(out, n).find("END_MARKER8899") != std::string::npos);
    }

    // The child inherits a full ring; after its raise(SIGSEGV) the handler
    // writes crash-<childpid>.log and _exits without running the atexit chain.
#if !defined(_WIN32)
    void FatalSignalDumpsCrashLog()
    {
        const std::string atexitProbe = "atexit_ran.log";
        std::remove(atexitProbe.c_str());

        SEKAI_INFO("pre-fork marker=CRASH_PROBE_KEEP_ME");

        const pid_t child = fork();
        CHECK(child >= 0);
        if (child == 0)
        {
            std::atexit([]() { std::ofstream("atexit_ran.log").close(); });
            std::raise(SIGSEGV);
            _exit(0);
        }

        int status = 0;
        waitpid(child, &status, 0);

        // the handler took the SIGSEGV and _exit(128 + SIGSEGV): the child
        // exited normally - it was NOT killed by an uncaught signal
        CHECK(WIFEXITED(status));
        CHECK(WEXITSTATUS(status) == 128 + SIGSEGV);
        CHECK(!Exists(atexitProbe));

        // the handler cached g_dumpPath (with this process's pid) at install;
        // the fork child inherits it, so the crash lands here under our pid
        const std::string crashPath = CrashPathFor(static_cast<unsigned long>(getpid()));
        CHECK(Exists(crashPath));
        const std::string content = ReadFile(crashPath);
        CHECK(content.find("SEKAI ENGINE CRASH") != std::string::npos);
        CHECK(content.find("CRASH_PROBE_KEEP_ME") != std::string::npos);

        std::remove(crashPath.c_str());
        std::remove(atexitProbe.c_str());
    }
#else
    // This deliberately crashes the process that runs it (access violation).
    // It only ever executes inside the probe spawned below, never in the main
    // self-check run.
    void CrashProbe()
    {
        SEKAI_INFO("pre-crash marker=WIN_VEH_PROBE");
        *reinterpret_cast<volatile int*>(0) = 42;
        std::fprintf(stderr, "FAIL: the vectored handler should have terminated this process\n");
        std::exit(1);
    }

    void WindowsExceptionDumpsCrashLog()
    {
        char selfExe[MAX_PATH];
        GetModuleFileNameA(nullptr, selfExe, sizeof(selfExe));

        std::string commandLine = std::string("\"") + selfExe + "\" --crash-probe";
        PROCESS_INFORMATION info = {};
        const BOOL created = CreateProcessA(selfExe, const_cast<char*>(commandLine.c_str()),
                                            nullptr, nullptr, TRUE, 0, nullptr, nullptr, nullptr, &info);
        CHECK(created != FALSE);
        if (!created)
        {
            return;
        }
        WaitForSingleObject(info.hProcess, 30000);

        DWORD exitCode = 0;
        GetExitCodeProcess(info.hProcess, &exitCode);
        const unsigned long childPid = GetProcessId(info.hProcess);
        CloseHandle(info.hProcess);
        CloseHandle(info.hThread);

        CHECK(exitCode != 0); // probe should have been terminated by the handler
        const std::string crashPath = CrashPathFor(childPid);
        CHECK(Exists(crashPath));
        if (Exists(crashPath))
        {
            const std::string content = ReadFile(crashPath);
            CHECK(content.find("SEKAI ENGINE CRASH") != std::string::npos);
            CHECK(content.find("WIN_VEH_PROBE") != std::string::npos);
            std::remove(crashPath.c_str());
        }
    }
#endif

    void WebConsoleChannelMapping()
    {
        using SekaiEngine::ConsoleChannel;
        using SekaiEngine::LogLevel;
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Trace)) == static_cast<int>(ConsoleChannel::Log));
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Debug)) == static_cast<int>(ConsoleChannel::Log));
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Info)) == static_cast<int>(ConsoleChannel::Log));
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Warning)) == static_cast<int>(ConsoleChannel::Warn));
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Error)) == static_cast<int>(ConsoleChannel::Error));
        CHECK(static_cast<int>(SekaiEngine::ChannelForLevel(LogLevel::Fatal)) == static_cast<int>(ConsoleChannel::Error));
    }

    void RaylibLevelMapping()
    {
        using SekaiEngine::LogLevel;
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(0)) == static_cast<int>(LogLevel::Info)); // LOG_ALL
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(1)) == static_cast<int>(LogLevel::Trace)); // LOG_TRACE
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(2)) == static_cast<int>(LogLevel::Debug)); // LOG_DEBUG
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(3)) == static_cast<int>(LogLevel::Info)); // LOG_INFO
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(4)) == static_cast<int>(LogLevel::Warning)); // LOG_WARNING
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(5)) == static_cast<int>(LogLevel::Error)); // LOG_ERROR
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(6)) == static_cast<int>(LogLevel::Fatal)); // LOG_FATAL
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(7)) == static_cast<int>(LogLevel::Info)); // LOG_NONE
        CHECK(static_cast<int>(SekaiEngine::RaylibLevelToEngineLevel(99)) == static_cast<int>(LogLevel::Info));
    }

    // A rolling file appender configured like the engine's (bounded size,
    // bounded count) rotates instead of growing without limit
    void RollingFileRotatesAndStaysBounded()
    {
        using Appender = plog::RollingFileAppender<plog::MessageOnlyFormatter>;
        const std::string path = UniquePath("rolling.log");
        std::remove(path.c_str());

        Appender appender(path.c_str(), 512, 3);
        char big[80] = {};
        std::memset(big, 'r', sizeof(big) - 1);
        for (int i = 0; i < 150; ++i)
        {
            plog::Record record(plog::Severity::info, "", 0, "", nullptr, 0);
            record << "rotation probe " << i << " " << big;
            appender.write(record);
        }

        long totalBytes = 0;
        for (int i = 0; i < 3; ++i)
        {
            const std::string name = (0 == i) ? path : path + "." + std::to_string(i);
            if (Exists(name))
            {
                totalBytes += static_cast<long>(ReadFile(name).size());
            }
        }

        // 150 * ~90 bytes would be ~13.5 kB unbounded; three capped files stay small
        CHECK(totalBytes < 10000L);
        std::remove(path.c_str());
        std::remove((path + ".1").c_str());
        std::remove((path + ".2").c_str());
    }
} // namespace

int main(int argc, char** argv)
{
#if defined(_WIN32)
    if (argc > 1 && 0 == std::strcmp(argv[1], "--crash-probe"))
    {
        CrashProbe(); // never returns
    }
#endif

    NoFileUntilRequested();
    LevelGateSuppressesAndRoutes();
    FormattedLineAndOrdinal();
    ConcurrentWritesProduceCompleteLines();
    RingKeepsNewestWhenFull();

    // The fatal-fault crash dump is exercised through the platform's own
    // mechanism: a forked child on POSIX, a spawned probe on Windows
    // (tasks 5.2 / 5.3).
#if defined(_WIN32)
    WindowsExceptionDumpsCrashLog();
#else
    FatalSignalDumpsCrashLog();
#endif

    WebConsoleChannelMapping();
    RaylibLevelMapping();
    RollingFileRotatesAndStaysBounded();

    if (g_failures == 0)
    {
        std::printf("LOG SELF-CHECK: %zu/%zu checks passed\n", g_checks, g_checks);
        return 0;
    }
    std::fprintf(stderr, "LOG SELF-CHECK: %zu of %zu checks failed\n", g_failures, g_checks);
    return 1;
}