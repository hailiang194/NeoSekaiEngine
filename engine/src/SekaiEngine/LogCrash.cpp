#include "SekaiEngine/LogCrash.h"

#include <atomic>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32) && !defined(PLATFORM_WEB)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif !defined(PLATFORM_WEB)
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#endif

namespace SekaiEngine
{
    namespace
    {
        constexpr size_t kRingCapacity = 256 * 1024;

        // ponytail: single global 256 KiB ring. Double the constant if a game
        // wants more crash history; it is a compile-time value on purpose.
        struct Capture
        {
            char data[kRingCapacity];
            std::atomic<size_t> total{0};
        };

        Capture g_capture;

        void CaptureAppend(const char* bytes, size_t len)
        {
            if (0 == len || len > kRingCapacity)
            {
                return;
            }

            size_t start = g_capture.total.load(std::memory_order_relaxed);
            while (!g_capture.total.compare_exchange_weak(start, start + len,
                    std::memory_order_relaxed, std::memory_order_relaxed))
            {
            }

            const size_t pos = start % kRingCapacity;
            const size_t first = (len <= kRingCapacity - pos) ? len : (kRingCapacity - pos);
            std::memcpy(g_capture.data + pos, bytes, first);
            if (first < len)
            {
                std::memcpy(g_capture.data, bytes + first, len - first);
            }

            g_capture.total.store(start + len, std::memory_order_release);
        }

        size_t CaptureCopyLatest(char* out, size_t cap)
        {
            const size_t total = g_capture.total.load(std::memory_order_acquire);
            const size_t available = (total < kRingCapacity) ? total : kRingCapacity;
            const size_t n = (available < cap) ? available : cap;
            if (0 == n)
            {
                return 0;
            }

            const size_t start = (total - n) % kRingCapacity;
            const size_t first = (n <= kRingCapacity - start) ? n : (kRingCapacity - start);
            std::memcpy(out, g_capture.data + start, first);
            if (first < n)
            {
                std::memcpy(out + first, g_capture.data, n - first);
            }
            return n;
        }

#if !defined(PLATFORM_WEB)
        constexpr size_t StrLen(const char* s)
        {
            size_t n = 0;
            while (s[n] != '\0')
            {
                ++n;
            }
            return n;
        }

        char g_dumpPath[512];
        size_t g_dumpPathLen = 0;

        void ResolveDumpPath()
        {
#if defined(_WIN32)
            char cwd[256];
            if (GetCurrentDirectoryA(sizeof(cwd), cwd) == 0)
            {
                cwd[0] = '\0';
            }
            const unsigned long pid = GetCurrentProcessId();
            const char separator = '\\';
#else
            char cwd[256];
            if (getcwd(cwd, sizeof(cwd)) == nullptr)
            {
                cwd[0] = '\0';
            }
            const long pid = static_cast<long>(getpid());
            const char separator = '/';
#endif
            int n = std::snprintf(g_dumpPath, sizeof(g_dumpPath), "%s%ccrash-%ld.log",
                                  cwd, separator, pid);
            if (n < 0 || static_cast<size_t>(n) >= sizeof(g_dumpPath))
            {
                g_dumpPath[0] = '\0';
                g_dumpPathLen = 0;
            }
            else
            {
                g_dumpPathLen = static_cast<size_t>(n);
            }
        }

        const char kCrashHeader[] = "SEKAI ENGINE CRASH: ";
        const char kNewline[] = "\n";

        size_t DumpWrite(int fd, const char* bytes, size_t len)
        {
            size_t written = 0;
            while (written < len)
            {
                const ssize_t n = ::write(fd, bytes + written, len - written);
                if (n <= 0)
                {
                    break;
                }
                written += static_cast<size_t>(n);
            }
            return written;
        }

        void DumpRing(int fd)
        {
            const char* data = g_capture.data;
            const size_t total = g_capture.total.load(std::memory_order_acquire);
            const size_t n = (total < kRingCapacity) ? total : kRingCapacity;
            if (0 == n)
            {
                return;
            }

            const size_t start = (total - n) % kRingCapacity;
            const size_t first = (n <= kRingCapacity - start) ? n : (kRingCapacity - start);
            DumpWrite(fd, data + start, first);
            if (first < n)
            {
                DumpWrite(fd, data, n - first);
            }
        }

#if defined(_WIN32)
        struct ExceptionName
        {
            DWORD code;
            const char* name;
        };

        const ExceptionName kExceptionTable[] = {
            { EXCEPTION_ACCESS_VIOLATION,      "ACCESS_VIOLATION"      },
            { EXCEPTION_INT_DIVIDE_BY_ZERO,    "INT_DIVIDE_BY_ZERO"    },
            { EXCEPTION_FLT_DIVIDE_BY_ZERO,    "FLT_DIVIDE_BY_ZERO"    },
            { EXCEPTION_ILLEGAL_INSTRUCTION,   "ILLEGAL_INSTRUCTION"   },
            { EXCEPTION_STACK_OVERFLOW,        "STACK_OVERFLOW"        },
        };

        const char* ExceptionNameForCode(DWORD code)
        {
            for (const ExceptionName& entry : kExceptionTable)
            {
                if (entry.code == code)
                {
                    return entry.name;
                }
            }
            return nullptr;
        }

        void DumpCrashFileWindows(const char* name)
        {
            HANDLE fd = CreateFileA(g_dumpPath, GENERIC_WRITE, 0, nullptr,
                                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (fd == INVALID_HANDLE_VALUE)
            {
                return;
            }

            SetFilePointer(fd, 0, nullptr, FILE_BEGIN);
            SetEndOfFile(fd);

            const char* parts[] = { kCrashHeader, name, kNewline, g_dumpPath, kNewline };
            const size_t lengths[] = { sizeof(kCrashHeader) - 1, StrLen(name), 1, g_dumpPathLen, 1 };
            for (size_t i = 0; i < sizeof(parts) / sizeof(parts[0]); ++i)
            {
                DWORD wrote = 0;
                WriteFile(fd, parts[i], static_cast<DWORD>(lengths[i]), &wrote, nullptr);
            }

            const char* data = g_capture.data;
            const size_t total = g_capture.total.load(std::memory_order_acquire);
            const size_t n = (total < kRingCapacity) ? total : kRingCapacity;
            if (n > 0)
            {
                const size_t start = (total - n) % kRingCapacity;
                const size_t first = (n <= kRingCapacity - start) ? n : (kRingCapacity - start);
                DWORD wrote = 0;
                WriteFile(fd, data + start, static_cast<DWORD>(first), &wrote, nullptr);
                if (first < n)
                {
                    WriteFile(fd, data, static_cast<DWORD>(n - first), &wrote, nullptr);
                }
            }

            CloseHandle(fd);
        }

        LONG WINAPI CrashVectoredHandler(EXCEPTION_POINTERS* info)
        {
            const char* name = ExceptionNameForCode(info->ExceptionRecord->ExceptionCode);
            if (nullptr == name)
            {
                return EXCEPTION_CONTINUE_SEARCH;
            }

            static volatile LONG locking;
            if (InterlockedCompareExchange(&locking, 1, 0) != 0)
            {
                TerminateProcess(GetCurrentProcess(), 1);
            }

            DumpCrashFileWindows(name);
            TerminateProcess(GetCurrentProcess(), 1);
            return EXCEPTION_CONTINUE_EXECUTION;
        }
#else
        struct SignalName
        {
            int signum;
            const char* name;
        };

        const SignalName kSignalTable[] = {
            { SIGSEGV, "SIGSEGV (invalid memory reference)" },
            { SIGABRT, "SIGABRT (abnormal termination)"      },
            { SIGFPE,  "SIGFPE (erroneous arithmetic)"       },
            { SIGILL,  "SIGILL (illegal instruction)"        },
            { SIGBUS,  "SIGBUS (bus error)"                  },
        };

        extern "C" void CrashSignalHandler(int signum)
        {
            static volatile sig_atomic_t locking;
            if (__sync_val_compare_and_swap(&locking, 0, 1) != 0)
            {
                _exit(128 + signum);
            }

            const char* name = nullptr;
            size_t nameLen = 0;
            for (const SignalName& entry : kSignalTable)
            {
                if (entry.signum == signum)
                {
                    name = entry.name;
                    nameLen = StrLen(entry.name);
                    break;
                }
            }
            if (nullptr == name)
            {
                _exit(128 + signum);
            }

            const int fd = ::open(g_dumpPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0)
            {
                DumpWrite(fd, kCrashHeader, sizeof(kCrashHeader) - 1);
                DumpWrite(fd, name, nameLen);
                DumpWrite(fd, kNewline, 1);
                DumpWrite(fd, g_dumpPath, g_dumpPathLen);
                DumpWrite(fd, kNewline, 1);
                DumpRing(fd);
                ::close(fd);
            }

            _exit(128 + signum);
        }

        void InstallSignalHandlers()
        {
            alignas(16) static char altStack[64 * 1024];
            stack_t ss;
            ss.ss_sp = altStack;
            ss.ss_size = sizeof(altStack);
            ss.ss_flags = 0;
            sigaltstack(&ss, nullptr);

            struct sigaction sa;
            memset(&sa, 0, sizeof(sa));
            sa.sa_handler = CrashSignalHandler;
            sa.sa_flags = SA_ONSTACK;
            sigemptyset(&sa.sa_mask);
            for (const SignalName& entry : kSignalTable)
            {
                sigaction(entry.signum, &sa, nullptr);
            }
        }
#endif
#endif // !defined(PLATFORM_WEB)
    } // namespace

    void LogCrash::Append(const char* bytes, size_t len)
    {
        CaptureAppend(bytes, len);
    }

    size_t LogCrash::CopyLatest(char* out, size_t cap)
    {
        return CaptureCopyLatest(out, cap);
    }

    void SekaiInstallCrashHandlers()
    {
#if !defined(PLATFORM_WEB)
        ResolveDumpPath();
#if defined(_WIN32)
        AddVectoredExceptionHandler(1, CrashVectoredHandler);
#else
        InstallSignalHandlers();
#endif
#endif
    }
} // namespace SekaiEngine