/**
 * @file Texture.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the wrapper functions for Raylib library texture
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _WRAPPER_RAYLIB_TEXTURE_H_
#define _WRAPPER_RAYLIB_TEXTURE_H_

#ifdef USE_RAYLIB

#include "raylib.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Get a loaded texture by its engine texture id
         *
         * @param id the id of the texture
         * @return the texture, or nullptr if no texture carries that id
         */
        ::Texture* getTexture(const int& id);
    } // namespace Render
    
} // namespace SekaiEngine

#endif


#endif//!_WRAPPER_RAYLIB_TEXTURE_H_