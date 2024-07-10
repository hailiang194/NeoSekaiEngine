/**
 * @file Font.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the wrapper functions for Font which calling to Raylib library
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _WRAPPER_RAYLIB_FONT_H_
#define _WRAPPER_RAYLIB_FONT_H_

#ifdef USE_RAYLIB
#include "raylib.h"

namespace SekaiEngine
{
    namespace Render
    {
        ::Font* getFontById(const char* id);
    } // namespace Render
    
} // namespace SekaiEngine

#endif

#endif//!_WRAPPER_RAYLIB_FONT_H_