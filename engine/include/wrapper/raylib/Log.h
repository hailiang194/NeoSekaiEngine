/**
 * @file Log.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Routes raylib's internal trace logs into the engine logger
 * @version 0.1
 * @date 2026-09-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef _SEKAI_ENGINE_WRAPPER_RAYLIB_LOG_H_
#define _SEKAI_ENGINE_WRAPPER_RAYLIB_LOG_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Log.h"

namespace SekaiEngine
{
    /**
     * @brief Map a raylib trace log level onto an engine level
     * 
     * raylib's TraceLogLevel is an unscoped enum (LOG_ALL=0, LOG_TRACE=1,
     * LOG_DEBUG=2, LOG_INFO=3, LOG_WARNING=4, LOG_ERROR=5, LOG_FATAL=6,
     * LOG_NONE=7). Kept as a free function so the mapping can be unit-tested
     * without linking raylib.
     * 
     * @param msgType one of the LOG_* integer levels
     * @return the matching engine level (unknown values map to Info)
     */
    EXTENDAPI LogLevel RaylibLevelToEngineLevel(int msgType);
} // namespace SekaiEngine

#ifdef USE_RAYLIB

namespace Wrapper
{
    namespace Raylib
    {
        /**
         * @brief Install the raylib -> engine log bridge
         * 
         * Must be called before InitWindow() so that warnings emitted during
         * window and graphics device initialization are captured.
         * 
         */
        void InstallRaylibLogBridge();
    } // namespace Raylib

} // namespace Wrapper

#endif//USE_RAYLIB

#endif //!_SEKAI_ENGINE_WRAPPER_RAYLIB_LOG_H_