#ifndef _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_
#define _SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/RendererAPI.h"

namespace SekaiEngine
{
    namespace Render
    {
        namespace RenderCommand
        {

            void StartDrawing(const Color& color);
            void FinishDrawing();

            inline void StartDrawing(const Color& color)
            {
                Render::API::BeginDrawing();
                Render::API::SetClearColor(color);
                Render::API::Clear();
            }

            inline void FinishDrawing()
            {
                Render::API::EndDrawing();
            }


        } // namespace RenderCommand
        
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_