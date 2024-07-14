/**
 * @file Circle.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The class represents circle
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SHAPE_CIRCLE_H_
#define _SEKAI_ENGINE_SHAPE_CIRCLE_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Shape/Shape.h"

namespace SekaiEngine
{
    namespace Shape
    {
        /**
         * @brief Circle class
         * 
         */
        class EXTENDAPI Circle: public IShape
        {
        public:
            /**
             * @brief Construct a new Circle object
             * 
             * @param center the center position of circle
             * @param radius the radius of circle
             */
            Circle(const Math::Vector2D& center = Math::Vector2D(), const float& radius = 0.0f);

            /**
             * @brief Construct a new Circle object
             * 
             * @param circle copied object
             */
            Circle(const Circle& circle);

            /**
             * @brief Copied assignment operator
             * 
             * @param circle copied object
             * @return Circle& the reference of the object itself
             */
            Circle& operator=(const Circle& circle);

            /**
             * @brief Destroy the Circle object
             * 
             */
            ~Circle();

            Math::Vector2D Center; /*!< The center of the circle*/
            float Radius; /*!< The radius of the circle*/
        };
    } // namespace Shape
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_SHAPE_CIRCLE_H_