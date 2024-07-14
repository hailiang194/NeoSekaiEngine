/**
 * @file RendererAPI.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The wrapper for rendering function
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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
            /**
             * @brief Set the Clear Color object
             * 
             * @param color the color of backgound
             */
            EXTENDAPI void SetClearColor(const Color& color);

            /**
             * @brief Clean screen, currently it does nothing
             */
            EXTENDAPI void Clear();

            /**
             * @brief Start drawing
             * 
             */
            EXTENDAPI void BeginDrawing();

            /**
             * @brief Finish drawring
             * 
             */
            EXTENDAPI void EndDrawing();

            /**
             * @brief Draw circle
             * 
             * @param circle the drawn circle
             * @param color the color of circle
             * @param startAngle the start angle to draw in degree
             * @param endAngle the end angle to draw in degree
             * @param segment segment of circle
             */
            EXTENDAPI void DrawCircle(const Shape::Circle& circle, const Color& color,
                const float& startAngle = 0.0f, const float& endAngle = 360.0f, const int& segment = 36
            );
#ifdef _MSC_VER

            /**
             * @brief Draw circle
             * 
             * @param circle the drawn circle
             * @param color the color of circle
             * @param startAngle the start angle to draw in degree
             * @param endAngle the end angle to draw in degree
             * @param segment segment of circle
             */
            EXTENDAPI void DrawCircle(const Shape::Circle& circle, const uint32_t& color,
                const float& startAngle = 0.0f, const float& endAngle = 360.0f, const int& segment = 36
            );
#endif
            /**
             * @brief Draw rectangle
             * 
             * @param rect drawn rectangle
             * @param color the color of rectangle
             * @param origin the rotation origin
             * @param rotation the rotation degree
             */
            EXTENDAPI void DrawRect(const Shape::Rectangle& rect, const Color& color,
                const Math::Vector2D& origin = Math::Vector2D(), const float& rotation = 0.0f
            );

#ifdef _MSC_VER
            /**
             * @brief Draw rectangle
             * 
             * @param rect drawn rectangle
             * @param color the color of rectangle
             * @param origin the rotation origin
             * @param rotation the rotation degree
             */
            EXTENDAPI void DrawRect(const Shape::Rectangle& rect, const uint32_t& color,
                const Math::Vector2D& origin = Math::Vector2D(), const float& rotation = 0.0f
            );
#endif

            /**
             * @brief Draw texture
             * 
             * @param texture the drawn texture
             * @param color the tint color
             * @param source the position and size of texture in the whole texture file
             * @param dest the drawn position and size 
             * @param origin the rotation origin
             * @param rotation the rotation degree
             */
            EXTENDAPI void DrawTexture(const Render::Texture& texture, 
                const Color& color,
                const Shape::Rectangle& source = FULL_RECTANGLE,
                const Shape::Rectangle& dest = FULL_RECTANGLE,
                const Math::Vector2D& origin = Math::Vector2D(),
                const float rotation = 0.0f
            );
#ifdef _MSC_VER
            /**
             * @brief Draw texture
             * 
             * @param texture the drawn texture
             * @param color the tint color
             * @param source the position and size of texture in the whole texture file
             * @param dest the drawn position and size 
             * @param origin the rotation origin
             * @param rotation the rotation degree
             */
            EXTENDAPI void DrawTexture(const Render::Texture& texture, 
                const uint32_t& color,
                const Shape::Rectangle& source = FULL_RECTANGLE,
                const Shape::Rectangle& dest = FULL_RECTANGLE,
                const Math::Vector2D& origin = Math::Vector2D(),
                const float rotation = 0.0f
            );
#endif
            /**
             * @brief Draw text
             * 
             * @param text the drawn text
             * @param postion the position to draw text
             * @param color the color of text
             * @param fontSize the size of font
             * @param font font of text
             * @param spacing the spacing between 2 characters
             */
            EXTENDAPI void DrawText(
                const char* text,
                const Math::Vector2D& postion, const Render::Color& color,
                const float& fontSize, const Render::Font& font = Render::Font("default"),
                const float& spacing = 1.0f    
            );
#ifdef _MSC_VER
            /**
             * @brief Draw text
             * 
             * @param text the drawn text
             * @param postion the position to draw text
             * @param color the color of text
             * @param fontSize the size of font
             * @param font font of text
             * @param spacing the spacing between 2 characters
             */
            EXTENDAPI void DrawText(
                const char* text,
                const Math::Vector2D& postion, const uint32_t& color,
                const float& fontSize, const Render::Font& font = Render::Font("default"),
                const float& spacing = 1.0f    
            );
#endif

            /**
             * @brief Draw bounding
             * 
             * @param bounding bounding area
             */
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