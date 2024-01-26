#include "SekaiEngine/Render/RendererAPI.h"

#ifdef USE_RAYLIB
#include "raylib.h"
#include "wrapper/raylib/Parser.h"
#include "wrapper/raylib/Texture.h"
#include <algorithm>

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
                ::EndDrawing();
            }

            void DrawCircle(const Shape::Circle& circle, const Color& color,
                const float& startAngle, const float& endAngle, const int& segment
            )
            {
                ::DrawCircleSector(::Vector2{ circle.Center.X(), circle.Center.Y() }, circle.Radius, 
                    startAngle, endAngle, segment, parseToRaylibColor(color)
                );
            }
            
            void DrawRect(const Shape::Rectangle& rect, const Color& color,
                const Math::Vector2D& origin, const float& rotation
            )
            {
                ::DrawRectanglePro(
                    ::Rectangle{ rect.Position.X(), rect.Position.Y(), rect.Width, rect.Height },
                    ::Vector2{ origin.X(), origin.Y() },
                    rotation, 
                    parseToRaylibColor(color)
                );
            }

            void DrawTexture(const Render::Texture& texture, const Color& color,
                const Shape::Rectangle& source, const Shape::Rectangle& dest, 
                const Math::Vector2D& origin, const float rotation
            )
            {
                ::Texture* rawTexture = SekaiEngine::Render::getTexture(texture.Id());

                ::Rectangle realSource = ::Rectangle {
                    std::max(source.Position.X(), 0.0f),
                    std::max(source.Position.Y(), 0.0f),
                    std::min(source.Width, static_cast<float>(rawTexture->width)),
                    std::min(source.Height, static_cast<float>(rawTexture->height)),
                };

                ::Rectangle realDest = ::Rectangle {
                    dest.Position.X(),
                    dest.Position.Y(),
                    dest.Width,
                    dest.Height
                };

                ::DrawTexturePro(*rawTexture, realSource, realDest, { origin.X(), origin.Y() }, rotation, parseToRaylibColor(color));
            }
        } // namespace API
        
    } // namespace Render
    
} // namespace SekaiEngine

#endif