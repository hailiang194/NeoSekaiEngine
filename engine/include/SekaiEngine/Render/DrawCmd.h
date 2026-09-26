/**
 * @file DrawCmd.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief A single draw, recorded as one self-contained value
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _SEKAI_ENGINE_RENDER_DRAW_CMD_H_
#define _SEKAI_ENGINE_RENDER_DRAW_CMD_H_

#include <type_traits>

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Render/Color.h"
#include "SekaiEngine/Render/RenderProperties.h"
#include "SekaiEngine/Render/Texture.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Shape/Rectangle.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief The kind of draw a DrawCmd carries, selecting the active union member
         *
         */
        enum class DrawTag
        {
            Circle, /*!< a circle or a circular sector*/
            Rectangle, /*!< a rectangle*/
            Texture, /*!< a texture or a region of one*/
            Text /*!< a text label*/
        };

        /**
         * @brief The payload of a circle draw
         *
         */
        struct CircleDraw
        {
            Shape::Circle circle; /*!< the drawn circle, already in world space*/
            float startAngle; /*!< the start angle to draw in degree*/
            float endAngle; /*!< the end angle to draw in degree*/
            int segment; /*!< the segment count of the arc*/
        };

        /**
         * @brief The payload of a rectangle draw
         *
         */
        struct RectDraw
        {
            Shape::Rectangle rect; /*!< the drawn rectangle, already in world space*/
            Math::Vector2D origin; /*!< the rotation origin*/
            float rotation; /*!< the rotation angle in degree*/
        };

        /**
         * @brief The payload of a texture draw
         *
         */
        struct TexDraw
        {
            Shape::Rectangle source; /*!< the region of the texture to read*/
            Shape::Rectangle dest; /*!< the drawn position and size, already in world space*/
            Math::Vector2D origin; /*!< the rotation origin*/
            float rotation; /*!< the rotation angle in degree*/
            Texture tex; /*!< the whole texture the source region is taken from*/
        };

        /**
         * @brief The payload of a text label draw
         *
         * @warning text and fontName must have static storage duration, or otherwise
         * outlive the frame's command buffer. A recorded draw is a value that the engine
         * does not own, so it stores these pointers rather than the bytes they name.
         * Recording text whose storage ends before the buffer is replayed replays a
         * dangling pointer and reads freed memory, which is why string literals satisfy
         * the contract and a runtime-composed string does not.
         *
         */
        struct TextDraw
        {
            const char* text; /*!< the drawn text, see the lifetime requirement above*/
            Math::Vector2D position; /*!< the drawn position, already in world space*/
            const char* fontName; /*!< the name the font face was loaded under*/
        };

        /**
         * @brief The union of every draw payload, discriminated by DrawCmd::tag
         *
         */
        union DrawVariant
        {
            /**
             * @brief Leave the variant without an active member
             *
             * @note No variant member is default constructible in a useful way - the
             * shapes have user-provided constructors and Texture has none at all - so
             * without this the union would have no default constructor and DrawCmd could
             * not be built. Every Make*Cmd function assigns the member its tag selects.
             */
            DrawVariant()
            {
            }

            CircleDraw circle; /*!< valid when the tag is DrawTag::Circle*/
            RectDraw rect; /*!< valid when the tag is DrawTag::Rectangle*/
            TexDraw tex; /*!< valid when the tag is DrawTag::Texture*/
            TextDraw text; /*!< valid when the tag is DrawTag::Text*/
        };

        /**
         * @brief One draw, self-contained and trivially copyable
         *
         */
        struct DrawCmd
        {
            DrawTag tag; /*!< selects the active member of the variant*/
            Color tint; /*!< the tint color of the draw*/
            DrawVariant variant; /*!< the payload the tag selects*/
        };

        /**
         * @brief Make the command for drawing a circle
         *
         * @param props render properties of the circle
         * @param circle the circle which needs drawing
         * @param startAngle the start angle to draw in degree
         * @param endAngle the end angle to draw in degree
         * @param segment the segment count of the arc
         * @return DrawCmd the command to record
         *
         * @note The radius is scaled by the X component of props.Scale only, so a
         * non-uniform scale produces a circle of the X-scaled size rather than an
         * ellipse. This matches the behaviour the render parameters class had, and
         * the circle renderer takes a single radius so it cannot express an ellipse.
         */
        EXTENDAPI DrawCmd MakeCircleCmd(const RenderProperties& props, const Shape::Circle& circle,
            const float& startAngle = 0.0f, const float& endAngle = 360.0f, const int& segment = 36
        );

        /**
         * @brief Make the command for drawing a rectangle
         *
         * @param props render properties of the rectangle
         * @param rect the rectangle which needs drawing
         * @return DrawCmd the command to record
         */
        EXTENDAPI DrawCmd MakeRectCmd(const RenderProperties& props, const Shape::Rectangle& rect);

        /**
         * @brief Make the command for drawing a texture
         *
         * @param props render properties of the texture
         * @param texture the whole texture needs drawing
         * @param position the position to draw the texture
         * @param atlasPosition the region of the texture to read, pass FULL_RECTANGLE to
         * draw the whole texture
         * @return DrawCmd the command to record
         *
         * @note The destination size comes from the dimensions Texture caches at load
         * time, so recording a texture draw touches no resource registry.
         */
        EXTENDAPI DrawCmd MakeTexCmd(const RenderProperties& props, const Texture& texture,
            const Math::Vector2D& position = Math::Vector2D(),
            const Shape::Rectangle& atlasPosition = FULL_RECTANGLE
        );

        /**
         * @brief Make the command for drawing a text label
         *
         * @param props render properties of the label
         * @param text the drawn text, see TextDraw for its lifetime requirement
         * @param position the position to draw the text
         * @param fontName the name the font face was loaded under
         * @return DrawCmd the command to record
         */
        EXTENDAPI DrawCmd MakeTextCmd(const RenderProperties& props, const char* text,
            const Math::Vector2D& position, const char* fontName
        );

        static_assert(std::is_trivially_copyable<DrawCmd>::value,
            "DrawCmd must stay trivially copyable so a recorded draw can be moved between threads");
        static_assert(sizeof(DrawCmd) <= 64, "DrawCmd must fit in one cache line");
    } // namespace Render

} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_DRAW_CMD_H_
