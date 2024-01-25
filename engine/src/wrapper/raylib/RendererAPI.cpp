#include "SekaiEngine/Render/RendererAPI.h"

#ifdef USE_RAYLIB
#include "raylib.h"
#include "wrapper/raylib/Parser.h"
namespace SekaiEngine
{
    namespace Render
    {
        namespace API
        {
            void SetClearColor(const Color& color)
            {
                ClearBackground(parseToRaylibColor(color));
            }

            void Clear()
            {
                
            }

            void BeginDrawing()
            {
                ::BeginDrawing();
            }

            void EndDrawing()
            {
                ::EndMode2D();
                ::EndDrawing();
            }
        } // namespace API
        
    } // namespace Render
    
} // namespace SekaiEngine

#endif