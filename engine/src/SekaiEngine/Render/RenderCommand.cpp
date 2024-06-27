#include "SekaiEngine/Render/RenderCommand.h"
#include "SekaiEngine/Math/Utility.h"
#include <array>

SekaiEngine::Math::Vector2D RotateVector(const SekaiEngine::Math::Vector2D& rotated, const float& degree)
{
    float radian = SekaiEngine::Math::DegreeToRadian(degree);

    float sinTheta = sinf(radian);
    float cosTheta = cosf(radian);

    return SekaiEngine::Math::Vector2D(
        rotated.X() * cosTheta - rotated.Y() * sinTheta, 
        rotated.X() * sinTheta + rotated.Y() * cosTheta
    );
}


namespace SekaiEngine
{
    namespace Render
    {
        namespace RenderCommand
        {
            

            Shape::Rectangle GetBoundingBox(const CircleRenderParams& param)
            {
                Math::Vector2D topLeft = param.DrawCircle().Center;
                topLeft.X() -= param.DrawCircle().Radius;
                topLeft.Y() -= param.DrawCircle().Radius;
                return Shape::Rectangle(
                    topLeft, 
                    param.DrawCircle().Radius * 2, 
                    param.DrawCircle().Radius * 2
                );
            }

            Shape::Rectangle GetBoundingBox(const RectangleRenderParams& param)
            {
                if(Math::cmpFloat(param.Rotation(), 0.0f) == 0)
                    return param.DrawRect();

                return Shape::Rectangle(Math::Vector2D(0.0f, 0.0f), 0.0f, 0.0f);
            }

            Shape::Rectangle GetBoundingBox(const TextureRenderParams& param)
            {
                if(Math::cmpFloat(param.Rotation(), 0.0f) == 0)
                    return param.Destination();

                return Shape::Rectangle(Math::Vector2D(0.0f, 0.0f), 0.0f, 0.0f);
            }
        } // namespace RenderCommand
        
    } // namespace Render
    
} // namespace SekaiEngine
