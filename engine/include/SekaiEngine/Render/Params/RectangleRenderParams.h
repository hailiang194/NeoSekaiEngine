/**
 * @file RectangleRenderParams.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the parameters for rendering a rectangle
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_PARAMS_RECTANGLE_RENDER_PARAMS_H_
#define _SEKAI_ENGINE_RENDER_PARAMS_RECTANGLE_RENDER_PARAMS_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/Params/RenderParams.h"
#include "SekaiEngine/Shape/Rectangle.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Render parameters for rendering a rectangle
         * 
         */
        class EXTENDAPI RectangleRenderParams: public RenderParams
        {
        public:
            /**
             * @brief Construct a new Rectangle Render Params object
             * 
             * @param props properties for rendering
             * @param rect the rectangle which needs drawing
             */
            RectangleRenderParams(const RenderProperties& props, const Shape::Rectangle& rect);
            RectangleRenderParams(const RectangleRenderParams& params) = delete;
            RectangleRenderParams& operator=(const RectangleRenderParams& params) = delete;
            /**
             * @brief Destroy the Rectangle Render Params object
             * 
             */
            ~RectangleRenderParams();

            /**
             * @brief Get drawn rectangle
             * 
             * @return const Shape::Rectangle& the drawn rectangle
             */
            const Shape::Rectangle& DrawRect() const;

            /**
             * @brief Get drawn rectangle
             * 
             * @return const Shape::Rectangle& the drawn rectangle
             */
            const Shape::Rectangle& DrawRect();

            /**
             * @brief the color of the drawn rectangle
             * 
             * @return const Render::Color& the color
             */
            const Render::Color& Tint() const;
            /**
             * @brief the color of the drawn rectangle
             * 
             * @return const Render::Color& the color
             */
            const Render::Color& Tint();

            /**
             * @brief The rotation origin of rectangle
             * 
             * @return const Math::Vector2D& the rotation origin
             */
            const Math::Vector2D& Origin() const;

            /**
             * @brief The rotation origin of rectangle
             * 
             * @return const Math::Vector2D& the rotation origin
             */            
            const Math::Vector2D& Origin();

            /**
             * @brief The rotation angle in degree
             * 
             * @return const float& the rotation angle
             */
            const float& Rotation() const;

            /**
             * @brief The rotation angle in degree
             * 
             * @return const float& the rotation angle
             */
            const float& Rotation();

        private:
            Shape::Rectangle m_drawRect; /*!< the drawn rectangle*/
        };

        inline const Shape::Rectangle& RectangleRenderParams::DrawRect() const
        {
            return m_drawRect;
        }

        inline const Shape::Rectangle& RectangleRenderParams::DrawRect()
        {
            return static_cast<const RectangleRenderParams&>(*this).DrawRect();
        }

        inline const Render::Color& RectangleRenderParams::Tint() const
        {
            return m_props.Tint;
        }

        inline const Render::Color& RectangleRenderParams::Tint()
        {
            return static_cast<const RectangleRenderParams&>(*this).Tint();
        }

        inline const Math::Vector2D& RectangleRenderParams::Origin() const
        {
            return m_props.Origin;
        }

        inline const Math::Vector2D& RectangleRenderParams::Origin()
        {
            return static_cast<const RectangleRenderParams&>(*this).Origin();
        }

        inline const float& RectangleRenderParams::Rotation() const
        {
            return m_props.Rotation;
        }

        inline const float& RectangleRenderParams::Rotation()
        {
            return static_cast<const RectangleRenderParams&>(*this).Rotation();
        }
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_PARAMS_RECTANGLE_RENDER_PARAMS_H_