#include "wrapper/raylib/Log.h"
#include "SekaiEngine/Log.h"

#ifdef USE_RAYLIB
#include "raylib.h"

#include <cstdio>
#include <cstdarg>

namespace SekaiEngine
{
    LogLevel RaylibLevelToEngineLevel(int msgType)
    {
        switch (msgType)
        {
            case LOG_TRACE:   return LogLevel::Trace;
            case LOG_DEBUG:   return LogLevel::Debug;
            case LOG_WARNING: return LogLevel::Warning;
            case LOG_ERROR:   return LogLevel::Error;
            case LOG_FATAL:   return LogLevel::Fatal;
            case LOG_INFO:
            default:          return LogLevel::Info;
        }
    }
}

namespace Wrapper
{
    namespace Raylib
    {
        namespace
        {
            void RaylibLogCallback(int logLevel, const char* text, va_list args)
            {
                char message[1024];
                vsnprintf(message, sizeof(message), text, args);
                SekaiEngine::Log::Write(SekaiEngine::RaylibLevelToEngineLevel(logLevel), "%s", message);
            }
        } // namespace

        void InstallRaylibLogBridge()
        {
            SetTraceLogCallback(RaylibLogCallback);
        }
    } // namespace Raylib

} // namespace Wrapper

#endif//USE_RAYLIB