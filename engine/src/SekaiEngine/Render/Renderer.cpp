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

        void Renderer::EndScreen(Camera2D& camera)
        {
            camera.unapplyCamera();
            RenderCommand::EndDrawing();
        }

        void Renderer::Render()
        {
            DrawText("Hello", 100, 100, 10, ::Color{ 230, 41, 55, 255 });
        }

        void Renderer::OnWindowResize(const int& width, const int& height)
        {

        }
    } // namespace Render
    
} // namespace SekaiEngine
