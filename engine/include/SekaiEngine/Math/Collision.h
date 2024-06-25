#ifndef _SEKAI_ENGINE_MATH_COLLISION_H_
#define _SEKAI_ENGINE_MATH_COLLISION_H_

#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Math/Vector.h"

namespace SekaiEngine
{
    namespace Math
    {
        EXTENDAPI bool isCollisionBetweenRectAndPoint(const Shape::Rectangle& rect, const Math::Vector2D& point);
        EXTENDAPI bool isCollisionBetweenCircleAndPoint(const Shape::Circle& circle, const Math::Vector2D& point);
        EXTENDAPI bool isCollisionBetween2Rects(const Shape::Rectangle& rect1, const Shape::Rectangle& rect2);
        EXTENDAPI bool isCollisionBetweenRectAndCircle(const Shape::Rectangle& rect, const Shape::Circle& circle);
    } // namespace Math
    
} // namespace SekaiEngine


#endif