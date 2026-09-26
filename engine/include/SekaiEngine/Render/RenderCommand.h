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
             * @brief Startup rendering for a frame
             *
             * @param color background color
             */
            void StartDrawing(const Color& color);

            /**
             * @brief Replay the frame's recorded draws, then finish rendering for a frame
             *
             */
            EXTENDAPI void FinishDrawing();

            /**
             * @brief Record a draw to be replayed at the end of the frame
             *
             * @param command the draw to record
             */
            EXTENDAPI void Record(const DrawCmd& command);

            inline void StartDrawing(const Color& color)
            {
                Render::API::BeginDrawing();
                Render::API::SetClearColor(color);
                Render::API::Clear();
            }
        } // namespace RenderCommand

    } // namespace Render

} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_
