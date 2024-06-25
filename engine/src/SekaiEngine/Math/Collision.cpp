#include "SekaiEngine/Math/Collision.h"
#include "SekaiEngine/Math/Utility.h"

namespace SekaiEngine
{
    namespace Math
    {
        bool isCollisionBetweenRectAndPoint(const Shape::Rectangle& rect, const Math::Vector2D& point)
        {
            return (
                point.X() >= rect.Position.X() &&
                point.X() <= rect.Position.X() + rect.Width &&
                point.Y() >= rect.Position.Y() &&
                point.Y() <= rect.Position.Y() + rect.Height
            );
        }

        bool isCollisionBetweenCircleAndPoint(const Shape::Circle& circle, const Math::Vector2D& point)
        {
            SekaiEngine::Math::Vector2D distance = point - circle.Center;

            return SekaiEngine::Math::cmpFloat(distance.Distance(), circle.Radius) <= 0;
        }

        bool isCollisionBetween2Rects(const Shape::Rectangle& rect1, const Shape::Rectangle& rect2)
        {
            return (
                rect1.Position.X() + rect1.Width >= rect2.Position.X() &&
                rect1.Position.X() <= rect2.Position.X() + rect2.Width &&
                rect1.Position.Y() + rect1.Height >= rect2.Position.Y() &&
                rect1.Position.Y() <= rect2.Position.Y() + rect2.Height
            );
        }

        bool isCollisionBetweenRectAndCircle(const Shape::Rectangle& rect, const Shape::Circle& circle)
        {
            //Get closest edge
            Math::Vector2D closestEdge = circle.Center;

            //find closest at X position
            if(circle.Center.X() < rect.Position.X())
                closestEdge.X() = rect.Position.X();
            else if (circle.Center.X() > rect.Position.X() + rect.Width)
                closestEdge.X() = rect.Position.X() + rect.Width;

            //find cloest at Y position
            if(circle.Center.Y() < rect.Position.Y())
                closestEdge.Y() = rect.Position.Y();
            else if (circle.Center.Y() > rect.Position.Y() + rect.Height)
                closestEdge.Y() = rect.Position.Y() + rect.Height;

            Math::Vector2D closest = closestEdge - circle.Center;

            return Math::cmpFloat(closest.Distance(), circle.Radius) <= 0;
        }
    } // namespace Math
    
} // namespace SekaiEngine
