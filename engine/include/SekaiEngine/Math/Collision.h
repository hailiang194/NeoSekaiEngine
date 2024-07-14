/**
 * @file Collision.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Check collision functions for shapes
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_MATH_COLLISION_H_
#define _SEKAI_ENGINE_MATH_COLLISION_H_

#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Math/Vector.h"

namespace SekaiEngine
{
    namespace Math
    {
        /**
         * @brief Check collision for a rectangle and point
         * 
         * @param rect checked rectangle
         * @param point checked point
         * @return bool true if 2 shapes are collision, otherwise false
         */
        EXTENDAPI bool isCollisionBetweenRectAndPoint(const Shape::Rectangle& rect, const Math::Vector2D& point);
        
        /**
         * @brief Check collision for a circle and point
         * 
         * @param circle checked circle
         * @param point checked point
         * @return bool true if 2 shapes are collision, otherwise false
         */
        EXTENDAPI bool isCollisionBetweenCircleAndPoint(const Shape::Circle& circle, const Math::Vector2D& point);
        
        /**
         * @brief Check collision for 2 rectangles
         * 
         * @param rect1 first rectangle
         * @param rect2 second rectangle
         * @return bool true if 2 shapes are collision, otherwise false
         */
        EXTENDAPI bool isCollisionBetween2Rects(const Shape::Rectangle& rect1, const Shape::Rectangle& rect2);
        
        /**
         * @brief Check collision for rectangle and circle
         * 
         * @param rect checked rectangle
         * @param circle checked circle
         * @return bool true if 2 shapes are collision, otherwise false
         */
        EXTENDAPI bool isCollisionBetweenRectAndCircle(const Shape::Rectangle& rect, const Shape::Circle& circle);
        
        /**
         * @brief Check collision for 2 circles
         * 
         * @param circle1 first circle
         * @param circle2 second circle
         * @return bool true if 2 shapes are collision, otherwise false
         */
        EXTENDAPI bool isCollisionBetween2Circles(const Shape::Circle& circle1, const Shape::Circle& circle2);
    } // namespace Math
    
} // namespace SekaiEngine


#endif