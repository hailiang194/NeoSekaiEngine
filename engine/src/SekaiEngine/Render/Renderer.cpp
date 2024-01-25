#include "SekaiEngine/Render/Renderer.h"
#include "SekaiEngine/Render/RenderCommand.h"

namespace SekaiEngine
{
    namespace Render
    {
        void Renderer::BeginScreen()
        {
            RenderCommand::BeginDrawing();
        }

        void Renderer::EndScreen()
        {
            RenderCommand::EndDrawing();
        }

        void Renderer::Render()
        {

        }

        void Renderer::OnWindowResize(const int& width, const int& height)
        {

        }
    } // namespace Render
    
} // namespace SekaiEngine
