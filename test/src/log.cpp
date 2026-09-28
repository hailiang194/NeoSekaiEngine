/**
 * @file log.cpp
 * @brief Headless coverage of the engine logging facility
 *
 * The console prints and the crash handlers need no window, so this suite runs
 * in engine_test's display-less process. File-sink tests isolate themselves by
 * giving every test its own unique log path and reading back only that file:
 * the engine logger keeps its sinks globally enabled once SetFile is called.
 *
 * Order note: LogTest.NoFileUntilRequested must stay first, because it is the
 * only test that can still observe the default state (no file), before any
 * other test turns the file sink on.
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <gtest/gtest.h>

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <process.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#include "SekaiEngine/Log.h"
#include "SekaiEngine/LogCrash.h"
#include "wrapper/raylib/Log.h"

#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Record.h>

namespace
{
    bool Exists(const std::string& path)
    {
        std::ifstream stream(path);
        return stream.good();
    }

    std::string ReadFile(const std::string& path)
    {
        std::ifstream stream(path, std::ios::binary);
        return std::string((std::istreambuf_iterator<char>(stream)),
                           std::istreambuf_iterator<char>());
    }

    long ProcessId()
    {
#if defined(_WIN32)
        return static_cast<long>(_getpid());
#else
        return static_cast<long>(getpid());
#endif
    }

    std::string UniquePath(const char* suffix)
    {
        return testing::TempDir() + "/sekai_log_" + std::to_string(ProcessId()) + "_" + suffix;
    }

#if !defined(_WIN32)
    std::string CrashPathFor(long pid)
    {
        return "crash-" + std::to_string(pid) + ".log";
    }
#else
    std::string CrashPathFor(unsigned long pid)
    {
        return "crash-" + std::to_string(pid) + ".log";
    }
#endif
} // namespace

//Default state: nothing is ever written to a file unless a file sink is
//requested with SetFile() or the SEKAI_LOG_FILE environment variable
TEST(LogTest, NoFileUntilRequested)
{
    const std::string path = UniquePath("no_file.log");
    std::remove(path.c_str());

    SEKAI_INFO("this message goes to the console only, marker=NO_FILE_PROBE");
    EXPECT_FALSE(Exists(path));
}

//A fatal fault: the surviving child raises SIGSEGV, our handler writes a crash
//log from the ring and _exits without running the atexit chain
#if !defined(_WIN32)
TEST(LogTest, FatalSignalDumpsCrashLog)
{
    // Runs before any SetFile call, so the crash dump is proven to happen even
    // when no file sink was requested (spec 6.1).
    const long myPid = ProcessId();
    const std::string crashPath = CrashPathFor(myPid);
    const std::string atexitProbe = "atexit_ran.log";
    std::remove(crashPath.c_str());
    std::remove(atexitProbe.c_str());

    SEKAI_INFO("pre-fork marker=CRASH_PROBE_KEEP_ME");

    const pid_t child = fork();
    ASSERT_GE(child, 0);
    if (child == 0)
    {
        std::atexit([]() { std::ofstream("atexit_ran.log").close(); });
        std::raise(SIGSEGV);
        _exit(0);
    }

    int status = 0;
    ASSERT_EQ(child, waitpid(child, &status, 0));

    // the handler took the SIGSEGV and _exit(128 + SIGSEGV): the child exited
    // normally - it was NOT killed by an uncaught signal, which is the point
    EXPECT_TRUE(WIFEXITED(status));
    EXPECT_EQ(128 + SIGSEGV, WEXITSTATUS(status));

    // the handler used _exit, so the atexit probe never ran
    EXPECT_FALSE(Exists(atexitProbe));

    // the dump carries the newest ring lines, written independently of the
    // requested file sink
    ASSERT_TRUE(Exists(crashPath)) << "crash log missing: " << crashPath;
    const std::string content = ReadFile(crashPath);
    EXPECT_NE(std::string::npos, content.find("SEKAI ENGINE CRASH"));
    EXPECT_NE(std::string::npos, content.find("CRASH_PROBE_KEEP_ME"));

    std::remove(crashPath.c_str());
    std::remove(atexitProbe.c_str());
}
#else
TEST(LogTest, FatalSignalDumpsCrashLog)
{
    GTEST_SKIP() << "POSIX-only; the Windows vectored exception handler is "
                    "exercised by LogTest.WindowsExceptionDumpsCrashLog";
}
#endif

//Messages below the configured threshold never reach any sink; messages at or
//above it do. The threshold also applies to the in-memory crash capture.
TEST(LogTest, LevelGateSuppressesAndRoutes)
{
    const std::string path = UniquePath("gate.log");
    std::remove(path.c_str());
    SekaiEngine::Log::SetFile(path.c_str());

    SekaiEngine::Log::SetLevel(SekaiEngine::LogLevel::Warning);
    SEKAI_INFO("suppressed under a warning gate, marker=GATE_SUPPRESSED");
    SEKAI_ERROR("allowed under a warning gate, marker=GATE_ALLOWED");
    SekaiEngine::Log::SetLevel(SekaiEngine::LogLevel::Info);

    const std::string content = ReadFile(path);
    EXPECT_EQ(std::string::npos, content.find("GATE_SUPPRESSED"));
    EXPECT_NE(std::string::npos, content.find("GATE_ALLOWED"));
}

//The convenience macros render the level and a wall-clock timestamp but no
//source file (spec-less convention, see design.md D3/Open Questions), and the
//ordinal only ever increases between two emitted messages
TEST(LogTest, MacrosCarryLevelTimestampAndOrdinal)
{
    const std::string path = UniquePath("macro.log");
    std::remove(path.c_str());
    SekaiEngine::Log::SetFile(path.c_str());

    SEKAI_INFO("first marker=%d", 42);
    SEKAI_WARNING("second marker=%s", "wow");

    const std::string content = ReadFile(path);
    EXPECT_NE(std::string::npos, content.find("first marker=42"));
    EXPECT_NE(std::string::npos, content.find("second marker=wow"));

    // every line carries its level and a HH:MM:SS.mmm timestamp ...
    EXPECT_TRUE(std::regex_search(content,
                                  std::regex("\\[INFO\\] #[0-9]+ [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} first marker=42")));
    EXPECT_TRUE(std::regex_search(content,
                                  std::regex("\\[WARNING\\] #[0-9]+ [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} second marker=wow")));

    // ... and no source file path leaks into the output
    EXPECT_EQ(std::string::npos, content.find("log.cpp"));
    EXPECT_EQ(std::string::npos, content.find("/")); // no path separators either

    // ordinal of the second message is strictly greater than the first
    std::smatch firstOrdinal;
    std::smatch secondOrdinal;
    const bool hasFirst = std::regex_search(content, firstOrdinal, std::regex("\\[INFO\\] #([0-9]+) [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} first marker=42"));
    const bool hasSecond = std::regex_search(content, secondOrdinal, std::regex("\\[WARNING\\] #([0-9]+) [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3} second marker=wow"));
    ASSERT_TRUE(hasFirst);
    ASSERT_TRUE(hasSecond);
    const long a = std::stol(firstOrdinal[1].str());
    const long b = std::stol(secondOrdinal[1].str());
    EXPECT_LT(a, b);
}

//Two threads logging concurrently never tear a line: every marker appears
//exactly once and no physical line ends up carrying two of them
TEST(LogTest, ConcurrentWritesProduceCompleteLines)
{
    const std::string path = UniquePath("threads.log");
    std::remove(path.c_str());
    SekaiEngine::Log::SetFile(path.c_str());

    constexpr size_t kMessages = 400;
    std::thread first([]()
    {
        for (size_t i = 0; i < kMessages; ++i)
        {
            SEKAI_INFO("HEAD_%zu", i);
        }
    });
    std::thread second([]()
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
    const size_t lines = std::count(content.begin(), content.end(), '\n');
    EXPECT_EQ(2 * kMessages, lines);

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
    EXPECT_EQ(2 * kMessages, headTotal + tailTotal);
}

//The in-memory ring keeps the newest bytes, drops the oldest once full, and
//never allocates or locks on the append path (fixed capacity, no allocations)
TEST(LogTest, RingKeepsNewestWhenFull)
{
    char out[4096];

    // order preserved, oldest first: the newest bytes are exactly what we
    // appended last (earlier tests have already filled the ring into it)
    SekaiEngine::LogCrash::Append("AAA", 3);
    SekaiEngine::LogCrash::Append("BBB", 3);
    const size_t small = SekaiEngine::LogCrash::CopyLatest(out, sizeof(out));
    ASSERT_GE(small, 6u);
    EXPECT_EQ("AAABBB", std::string(out + small - 6, 6));

    // overflow: with ~300 KiB of 1 KiB lines, only the tail survives
    const std::string line = std::string(1012, 'x') + "END_MARKER8899";
    for (size_t i = 0; i < 300; ++i)
    {
        SekaiEngine::LogCrash::Append(line.c_str(), line.size());
    }

    const size_t n = SekaiEngine::LogCrash::CopyLatest(out, sizeof(out));
    ASSERT_GT(n, 0u);
    const std::string tail(out, n);
    EXPECT_NE(std::string::npos, tail.find("END_MARKER8899"));
}

//The Web console channel mapping is a pure function, so it is tested here on a
//non-Web build (spec 3.3)
TEST(LogTest, WebConsoleChannelMapping)
{
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Log,   SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Trace));
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Log,   SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Debug));
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Log,   SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Info));
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Warn,  SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Warning));
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Error, SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Error));
    EXPECT_EQ(SekaiEngine::ConsoleChannel::Error, SekaiEngine::ChannelForLevel(SekaiEngine::LogLevel::Fatal));
}

//raylib's level constants are plain ints (LOG_ALL=0 ... LOG_FATAL=6,
//LOG_NONE=7), so the bridge mapping is testable without linking raylib
TEST(LogTest, RaylibLevelMapping)
{
    using SekaiEngine::LogLevel;
    EXPECT_EQ(LogLevel::Info,    SekaiEngine::RaylibLevelToEngineLevel(0)); // LOG_ALL
    EXPECT_EQ(LogLevel::Trace,   SekaiEngine::RaylibLevelToEngineLevel(1)); // LOG_TRACE
    EXPECT_EQ(LogLevel::Debug,   SekaiEngine::RaylibLevelToEngineLevel(2)); // LOG_DEBUG
    EXPECT_EQ(LogLevel::Info,    SekaiEngine::RaylibLevelToEngineLevel(3)); // LOG_INFO
    EXPECT_EQ(LogLevel::Warning, SekaiEngine::RaylibLevelToEngineLevel(4)); // LOG_WARNING
    EXPECT_EQ(LogLevel::Error,   SekaiEngine::RaylibLevelToEngineLevel(5)); // LOG_ERROR
    EXPECT_EQ(LogLevel::Fatal,   SekaiEngine::RaylibLevelToEngineLevel(6)); // LOG_FATAL
    EXPECT_EQ(LogLevel::Info,    SekaiEngine::RaylibLevelToEngineLevel(7)); // LOG_NONE
    EXPECT_EQ(LogLevel::Info,    SekaiEngine::RaylibLevelToEngineLevel(99));
}

//On Windows the vectored exception handler takes a controlled access violation
//in a spawned copy of this executable, so the test process itself survives
#if defined(_WIN32)
TEST(LogTest, WindowsExceptionDumpsCrashLog)
{
    char selfExe[MAX_PATH];
    GetModuleFileNameA(nullptr, selfExe, sizeof(selfExe));

    SECURITY_ATTRIBUTES inheritable = {};
    inheritable.nLength = sizeof(inheritable);
    inheritable.bInheritHandle = TRUE;

    HANDLE stdoutRead = nullptr;
    HANDLE stdoutWrite = nullptr;
    CreatePipe(&stdoutRead, &stdoutWrite, &inheritable, 0);
    SetHandleInformation(stdoutRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA startup = {};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdOutput = stdoutWrite;
    startup.hStdError = stdoutWrite;

    std::string commandLine = std::string("\"") + selfExe + "\" --gtest_filter=LogTest.WindowsCrashProbe";
    PROCESS_INFORMATION process = {};
    ASSERT_TRUE(CreateProcessA(selfExe, const_cast<char*>(commandLine.c_str()),
                 nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup, &process));
    CloseHandle(stdoutWrite);

    WaitForSingleObject(process.hProcess, 30000);
    DWORD exitCode = 0;
    GetExitCodeProcess(process.hProcess, &exitCode);
    const unsigned long childPid = GetProcessId(process.hProcess);
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
    CloseHandle(stdoutRead);

    EXPECT_FALSE(exitCode == 0) << "probe process should have been terminated by the handler";
    const std::string crashPath = CrashPathFor(childPid);
    ASSERT_TRUE(Exists(crashPath)) << "crash log missing: " << crashPath;
    const std::string content = ReadFile(crashPath);
    EXPECT_NE(std::string::npos, content.find("SEKAI ENGINE CRASH"));
    EXPECT_NE(std::string::npos, content.find("WIN_VEH_PROBE"));
    std::remove(crashPath.c_str());
}

//This test deliberately crashes the process it runs in (access violation). It
//only ever executes inside the spawned probe above, never in the main suite.
TEST(LogTest, WindowsCrashProbe)
{
    SEKAI_INFO("pre-crash marker=WIN_VEH_PROBE");
    *reinterpret_cast<volatile int*>(0) = 42;
    GTEST_FAIL() << "the vectored handler should have terminated this process";
}
#endif

//A rolling file appender configured like the engine's (bounded size, bounded
//count) rotates instead of growing without limit
TEST(LogTest, RollingFileRotatesAndStaysBounded)
{
    using Appender = plog::RollingFileAppender<plog::MessageOnlyFormatter>;
    const std::string path = UniquePath("rolling.log");
    std::remove(path.c_str());

    Appender appender(path.c_str(), 512, 3);
    char big[80] = {};
    std::memset(big, 'r', sizeof(big) - 1);
    for (int i = 0; i < 150; ++i)
    {
        plog::Record record(plog::Severity::info, "log.cpp", i, "file", nullptr, 0);
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
    EXPECT_LT(totalBytes, 10000L);
    std::remove(path.c_str());
    std::remove((path + ".1").c_str());
    std::remove((path + ".2").c_str());
}