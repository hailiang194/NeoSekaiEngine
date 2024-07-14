/**
 * @file Rectangle.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The class represents rectangle
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SHAPE_RECTANGLE_H_
#define _SEKAI_ENGINE_SHAPE_RECTANGLE_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Shape/Shape.h"
#include "SekaiEngine/Math/Vector.h"
#include <limits>

/**
 * @brief define the full rectangle 
 * 
 */
#define FULL_RECTANGLE SekaiEngine::Shape::Rectangle( \
    Math::Vector2D(std::numeric_limits<float>::min(), std::numeric_limits<float>::min()), \
    std::numeric_limits<float>::max(), std::numeric_limits<float>::max() \
)

namespace SekaiEngine
{
    namespace Shape
    {
        /**
         * @brief Rectangle class
         * 
         */
        class EXTENDAPI Rectangle: public IShape
        {
        public:
            /**
             * @brief Construct a new Rectangle object
             * 
             * @param position the top left position of rectangle
             * @param width the width of rectangle
             * @param height the height of rectangle
             */
            Rectangle(const Math::Vector2D& position = Math::Vector2D(), const float& width = 0.0f, const float& height = 0.0f);

            /**
             * @brief Construct a new Rectangle object
             * 
             * @param rect copied object
             */
            Rectangle(const Rectangle& rect);

            /**
             * @brief Copied assignment operator
             * 
             * @param rect copied object
             * @return Rectangle& the reference of the object itself
             */
            Rectangle& operator=(const Rectangle& rect);

            /**
             * @brief Destroy the Rectangle object
             * 
             */
            ~Rectangle();

            Math::Vector2D Position; /*!< The top left position of the rectangle*/
            float Width; /*!< The width of the rectangle*/
            float Height; /*!< The height of the rectangle*/
        };
    } // namespace Shape
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_SHAPE_RECTANGLE_H_