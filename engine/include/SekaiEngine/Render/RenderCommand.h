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
            void SetClearColor(const Color& color);
            void Clear();
            void BeginDrawing();
            void EndDrawing();

            inline void SetClearColor(const Color& color)
            {
                Render::API::SetClearColor(color);
            }

            inline void Clear()
            {
                Render::API::Clear();
            }

            inline void BeginDrawing()
            {
                Render::API::BeginDrawing();
            }

            inline void EndDrawing()
            {
                Render::API::EndDrawing();
            }

        } // namespace RenderCommand
        
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_COMMAND_H_