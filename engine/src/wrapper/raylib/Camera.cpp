#include "SekaiEngine/Render/Camera.h"

#ifdef USE_RAYLIB
#include "raylib.h"

namespace SekaiEngine
{
    namespace Render
    {
        void Camera2D::applyCamera()
        {
            ::Camera2D camera;
            camera.target = { Position.X(), Position.Y() };
            camera.offset = { Offset.X(), Offset.Y() };
            camera.rotation = Rotation;
            camera.zoom = Zoom;
            BeginMode2D(camera);
        }

    } // namespace Render
    
} // namespace SekaiEngine


#endif 