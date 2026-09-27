/**
 * @file RenderCommand.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the command for rendering
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_
#define _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_

#include <vector>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/DrawCmd.h"
#include "SekaiEngine/Render/RendererAPI.h"

namespace SekaiEngine
{
    namespace Render
    {
        namespace RenderCommand
        {

            /**
             * @brief Start appending recorded draws to the frame's buffer
             *
             * @param commands the buffer this thread records the frame into
             *
             * @note The buffer is a parameter rather than engine state so the one being
             * written is visible at the call site and cannot be confused with the one
             * being replayed. Record only appends between BeginRecording and EndRecording.
             */
            EXTENDAPI void BeginRecording(std::vector<DrawCmd>& commands);

            /**
             * @brief Stop recording, so a later Record is reported and dropped
             *
             */
            EXTENDAPI void EndRecording();

            /**
             * @brief Replay a frame's recorded draws and bracket the frame
             *
             * @param commands the frame's recorded draws, in the order they were recorded
             * @param clearColor background color
             *
             * @note Must run on the thread that created the window, which owns the
             * graphics context. One call brackets the whole frame, so a caller cannot
             * begin a frame without ending it.
             */
            EXTENDAPI void ReplayFrame(const std::vector<DrawCmd>& commands, const Color& clearColor);

            /**
             * @brief Record a draw to be replayed at the end of the frame
             *
             * @param command the draw to record
             */
            EXTENDAPI void Record(const DrawCmd& command);
        } // namespace RenderCommand

    } // namespace Render

} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_
