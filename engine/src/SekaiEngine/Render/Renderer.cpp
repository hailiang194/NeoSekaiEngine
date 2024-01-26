#include "raylib.h"
#include "SekaiEngine/Render/Renderer.h"
#include "SekaiEngine/Render/RenderCommand.h"


namespace SekaiEngine
{
    namespace Render
    {
        void Renderer::BeginScreen(Camera2D& camera)
        {
            RenderCommand::BeginDrawing();
            camera.applyCamera();
        }

        void Renderer::EndScreen()
        {
            RenderCommand::EndDrawing();
        }

        void Renderer::OnWindowResize(const int& width, const int& height)
        {

        }
    } // namespace Render
    
} // namespace SekaiEngine
