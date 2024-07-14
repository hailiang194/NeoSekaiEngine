/**
 * @file CircleRenderParams.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the parameters for rendering a circle
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_PARAMS_CIRCLE_RENDER_PARAMS_H_
#define _SEKAI_ENGINE_RENDER_PARAMS_CIRCLE_RENDER_PARAMS_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/Params/RenderParams.h"
#include "SekaiEngine/Shape/Circle.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Render parameters for rendering circle
         * 
         */
        class EXTENDAPI CircleRenderParams: public RenderParams
        {
        public:
            /**
             * @brief Construct a new Circle Render Params object
             * 
             * @param props render properties for rendering circle
             * @param circle the circle which needs drawing
             */
            CircleRenderParams(const RenderProperties& props, const Shape::Circle& circle);
            CircleRenderParams(const CircleRenderParams& params) = delete;
            CircleRenderParams& operator=(const CircleRenderParams& params) = delete;

            /**
             * @brief Destroy the Circle Render Params object
             * 
             */
            ~CircleRenderParams();

            /**
             * @brief The drawn circle
             * 
             * @return const Shape::Circle& the drawn circle
             */
            const Shape::Circle& DrawCircle() const;

            /**
             * @brief the drawn circle
             * 
             * @return const Shape::Circle& the drawn circle
             */
            const Shape::Circle& DrawCircle();

            /**
             * @brief the color of circle
             * 
             * @return const Render::Color& the color
             */
            const Render::Color& Tint() const;
            
            /**
             * @brief the color of circle
             * 
             * @return const Render::Color& the color
             */
            const Render::Color& Tint();


            /**
             * @brief the start angle to render the circle in degree
             * 
             * @return const float& the start angle
             */
            const float& StartAngle() const;

            /**
             * @brief the start angle to render the circle in degree
             * 
             * @return const float& the start angle
             */
            const float& StartAngle();
            /**
             * @brief the end angle to render the circle in degree
             * 
             * @return const float& the end angle
             */
            const float& EndAngle() const;

            /**
             * @brief the end angle to render the circle in degree
             * 
             * @return const float& the end angle
             */
            const float& EndAngle();

            /**
             * @brief The segment of rendering circle
             * 
             * @return const int& the segment
             */
            const int& Segment() const;

            /**
             * @brief the segment of rendering circle
             * 
             * @return const int& the segment
             */
            const int& Segment();

        private:
            Shape::Circle m_drawCircle; /*!< the drawn circle*/
            float m_startAngle; /*!< the start angle to render circle in degree*/
            float m_endAngle; /*!< the end angle to render circle in degree*/
            int m_segment; /*!< The segment of circle*/
        };


        inline const Shape::Circle& CircleRenderParams::DrawCircle() const
        {
            return m_drawCircle;
        }

        inline const Shape::Circle& CircleRenderParams::DrawCircle()
        {
            return static_cast<const CircleRenderParams&>(*this).DrawCircle();
        }

        inline const Render::Color& CircleRenderParams::Tint() const
        {
            return m_props.Tint;
        }

        inline const Render::Color& CircleRenderParams::Tint()
        {
            return static_cast<const CircleRenderParams&>(*this).Tint();
        }

        inline const float& CircleRenderParams::StartAngle() const
        {
            return m_startAngle;
        }

        inline const float& CircleRenderParams::StartAngle()
        {
            return static_cast<const CircleRenderParams&>(*this).StartAngle();
        }

        inline const float& CircleRenderParams::EndAngle() const
        {
            return m_endAngle;
        }

        inline const float& CircleRenderParams::EndAngle()
        {
            return static_cast<const CircleRenderParams&>(*this).EndAngle();
        }

        inline const int& CircleRenderParams::Segment() const
        {
            return m_segment;
        }

        inline const int& CircleRenderParams::Segment()
        {
            return static_cast<const CircleRenderParams&>(*this).Segment();
        }
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_PARAMS_CIRCLE_RENDER_PARAMS_H_