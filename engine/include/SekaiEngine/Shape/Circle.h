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
        class EXTENDAPI Circle: public IShape
        {
        public:
            Circle(const Math::Vector2D& center = Math::Vector2D(), const float& radius = 0.0f);
            Circle(const Circle& circle);
            Circle& operator=(const Circle& circle);
            ~Circle();

            Math::Vector2D Center;
            float Radius;
        };
    } // namespace Shape
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_SHAPE_CIRCLE_H_