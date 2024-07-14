/**
 * @file Renderer.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Event handler for rendering
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_RENDERER_H_
#define _SEKAI_ENGINE_RENDER_RENDERER_H_

#include "SekaiEngine/Render/Camera.h"
#include "SekaiEngine/Render/RenderProperties.h"
#include "SekaiEngine/Shape/Circle.h"
#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Render/Texture.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Render class (not yet useful)
         * 
         */
        class EXTENDAPI Renderer
        {
        public:
            /**
             * @brief handle event for window resize
             * 
             * @param width new width
             * @param height new height
             */
            static void OnWindowResize(const int& width, const int& height);
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDERER_H_