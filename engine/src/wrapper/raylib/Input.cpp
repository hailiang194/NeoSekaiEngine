#include "SekaiEngine/Input.h"
#include "raylib.h"

namespace SekaiEngine
{
    namespace Input
    {
        
        bool IsKeyPressed(int key)
        {
            return ::IsKeyDown(key);
        }

        bool IsButtonPressed(int key)
        {
            return ::IsMouseButtonDown(key);
        }

        float GetMouseX()
        {
            return static_cast<float>(::GetMouseX());
        }

        float GetMouseY()
        {
            return static_cast<float>(::GetMouseY());
        }

        size_t GetTotalTouchPoints()
        {
            
            return static_cast<size_t>((::GetGestureDetected() == GESTURE_NONE) ? 0 : ::GetTouchPointCount());
        }

        Math::Vector2D GetTouchPointPosition(const size_t& index)
        {
            ::Vector2 pos = ::GetTouchPosition(static_cast<int>(index));

            return Math::Vector2D(pos.x, pos.y);
        }
    } // namespace Input
    
} // namespace SekaiEngine
