#ifndef _SEKAI_ENGINE_INPUT_H_
#define _SEKAI_ENGINE_INPUT_H_

#include "SekaiEngine/BaseType.h"
#ifdef USE_RAYLIB
#include "raylib.h"
#endif

namespace SekaiEngine
{
    namespace Input
    {
        EXTENDAPI bool IsKeyPressed(int key);
        EXTENDAPI bool IsButtonPressed(int key);
        EXTENDAPI float GetMouseX();
        EXTENDAPI float GetMouseY();
    }
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_INPUT_H_