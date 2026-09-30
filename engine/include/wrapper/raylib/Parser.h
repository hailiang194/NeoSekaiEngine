/**
 * @file Parser.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the parser functions to Raylib datatype
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _WRAPPER_RAYLIB_PARSER_H_
#define _WRAPPER_RAYLIB_PARSER_H_

#ifdef USE_RAYLIB
#include "SekaiEngine/Render/Color.h"
#include "raylib.h"

/**
 * @brief Convert an engine color into the raylib color type
 *
 * Both types store RGBA in the same 32-bit code, so this is a copy of the
 * 4 bytes with no per-channel arithmetic.
 *
 * @param color the engine color
 * @return the equivalent raylib color
 */
::Color parseToRaylibColor(const SekaiEngine::Render::Color& color);
#endif

#endif //!_WRAPPER_RAYLIB_PARSER_H_