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
    } // namespace Math
    
} // namespace SekaiEngine
