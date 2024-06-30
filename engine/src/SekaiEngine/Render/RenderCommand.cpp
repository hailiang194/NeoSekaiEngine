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

SekaiEngine::Shape::Rectangle GetBoundRect(const float&width, const float& height, const SekaiEngine::Math::Vector2D& origin, const float& degree)
{
#define FOR_EACH_POINT_OF_BOUNDING \
    for(std::array<SekaiEngine::Math::Vector2D, 4>::iterator iter = points.begin(); iter != points.end(); ++iter)


    std::array<SekaiEngine::Math::Vector2D, 4> points = {
        SekaiEngine::Math::Vector2D(0.0f, 0.0f),
        SekaiEngine::Math::Vector2D(width, 0.0f),
        SekaiEngine::Math::Vector2D(0.0f, height),
        SekaiEngine::Math::Vector2D(width, height)
    };

    FOR_EACH_POINT_OF_BOUNDING
    {
        (*iter) = RotateVector((*iter) - origin, degree);
    }

    //get bounding
    SekaiEngine::Math::Vector2D topLeft = *points.begin();
    SekaiEngine::Math::Vector2D bottomRight = *points.begin();
    FOR_EACH_POINT_OF_BOUNDING
    {
        //update topleft
        if(topLeft.X() > iter->X())
            topLeft.X() = iter->X();
        if(topLeft.Y() > iter->Y())
            topLeft.Y() = iter->Y();
        //update bottomRight
        if(bottomRight.X() < iter->X())
            bottomRight.X() = iter->X();
        if(bottomRight.Y() < iter->Y())
            bottomRight.Y() = iter->Y();            
    }

    return SekaiEngine::Shape::Rectangle(topLeft, bottomRight.X() - topLeft.X(), bottomRight.Y() - topLeft.Y());
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


                SekaiEngine::Shape::Rectangle bound = GetBoundRect(param.DrawRect().Width, param.DrawRect().Height, param.Origin(), param.Rotation());
                bound.Position = bound.Position + param.DrawRect().Position;
                bound.Position = bound.Position + param.Origin();
                return bound;
            }

            Shape::Rectangle GetBoundingBox(const TextureRenderParams& param)
            {
                if(Math::cmpFloat(param.Rotation(), 0.0f) == 0)
                    return param.Destination();

                SekaiEngine::Shape::Rectangle bound = GetBoundRect(param.Destination().Width, param.Destination().Height, param.Origin(), param.Rotation());
                bound.Position = bound.Position + param.Destination().Position;
                bound.Position = bound.Position + param.Origin();
                return bound;
            }
        } // namespace RenderCommand
        
    } // namespace Render
    
} // namespace SekaiEngine
