#ifndef _SEKAI_ENGINE_RENDER_RENDERER_H_
#define _SEKAI_ENGINE_RENDER_RENDERER_H_

namespace SekaiEngine
{
    namespace Render
    {
        class Renderer
        {
        public:
            static void BeginScreen();
            static void EndScreen();
            static void Render();
            static void OnWindowResize(const int& width, const int& height);
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDERER_H_