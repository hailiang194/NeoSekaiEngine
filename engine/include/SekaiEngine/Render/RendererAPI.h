#ifndef _SEKAI_ENGINE_RENDER_REDNERER_API_H_
#define _SEKAI_ENGINE_RENDER_REDNERER_API_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/Color.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Render/Texture.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Render/Font.h"

namespace SekaiEngine
{
    namespace Render
    {
        namespace API
        {
            EXTENDAPI void SetClearColor(const Color& color);
            EXTENDAPI void Clear();
            EXTENDAPI void BeginDrawing();
            EXTENDAPI void EndDrawing();
            EXTENDAPI void DrawCircle(const Shape::Circle& circle, const Color& color,
                const float& startAngle = 0.0f, const float& endAngle = 360.0f, const int& segment = 36
            );
#ifdef _MSC_VER
            EXTENDAPI void DrawCircle(const Shape::Circle& circle, const uint32_t& color,
                const float& startAngle = 0.0f, const float& endAngle = 360.0f, const int& segment = 36
            );
#endif
            
            EXTENDAPI void DrawRect(const Shape::Rectangle& rect, const Color& color,
                const Math::Vector2D& origin = Math::Vector2D(), const float& rotation = 0.0f
            );

#ifdef _MSC_VER
            EXTENDAPI void DrawRect(const Shape::Rectangle& rect, const uint32_t& color,
                const Math::Vector2D& origin = Math::Vector2D(), const float& rotation = 0.0f
            );
#endif

            EXTENDAPI void DrawTexture(const Render::Texture& texture, 
                const Color& color,
                const Shape::Rectangle& source = FULL_RECTANGLE,
                const Shape::Rectangle& dest = FULL_RECTANGLE,
                const Math::Vector2D& origin = Math::Vector2D(),
                const float rotation = 0.0f
            );
#ifdef _MSC_VER
            EXTENDAPI void DrawTexture(const Render::Texture& texture, 
                const uint32_t& color,
                const Shape::Rectangle& source = FULL_RECTANGLE,
                const Shape::Rectangle& dest = FULL_RECTANGLE,
                const Math::Vector2D& origin = Math::Vector2D(),
                const float rotation = 0.0f
            );
#endif
            EXTENDAPI void DrawText(
                const char* text,
                const Math::Vector2D& postion, const Render::Color& color,
                const float& fontSize, const Render::Font& font = Render::Font("default"),
                const float& spacing = 1.0f    
            );
#ifdef _MSC_VER
            EXTENDAPI void DrawText(
                const char* text,
                const Math::Vector2D& postion, const uint32_t& color,
                const float& fontSize, const Render::Font& font = Render::Font("default"),
                const float& spacing = 1.0f    
            );
#endif

            EXTENDAPI void DrawBounding(const Shape::Rectangle& bounding);

#ifdef _MSC_VER
            inline void DrawCircle(const Shape::Circle& circle, const uint32_t& color,
                const float& startAngle, const float& endAngle, const int& segment
            )
            {
                DrawCircle(circle, (SekaiEngine::Render::Color)color, startAngle, endAngle, segment);
            }

            inline void DrawRect(const Shape::Rectangle& rect, const uint32_t& color,
                const Math::Vector2D& origin, const float& rotation
            )
            {
                DrawRect(rect, (SekaiEngine::Render::Color)color, origin, rotation);
            }

            inline void SekaiEngine::Render::API::DrawTexture(
                const Render::Texture & texture, const uint32_t & color, 
                const Shape::Rectangle & source, const Shape::Rectangle & dest, 
                const Math::Vector2D & origin, const float rotation
            )
            {
                return DrawTexture(texture, (SekaiEngine::Render::Color)color,
                    source, dest, origin, rotation
                );
            }

            inline void DrawText(
                const char* text,
                const Math::Vector2D& postion, const uint32_t& color,
                const float& fontSize, const Render::Font& font,
                const float& spacing
            )
            {
                DrawText(text, postion, (SekaiEngine::Render::Color)color, fontSize, font, spacing);
            }
#endif
        } // namespace API
        
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_REDNERER_API_H_