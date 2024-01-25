#ifndef _SEKAI_ENGINE_RENDER_REDNERER_API_H_
#define _SEKAI_ENGINE_RENDER_REDNERER_API_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/Color.h"

namespace SekaiEngine
{
    namespace Render
    {
        namespace API
        {
            void SetClearColor(const Color& color);
            void Clear();
            void BeginDrawing();
            void EndDrawing();
        } // namespace API
        
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_REDNERER_API_H_