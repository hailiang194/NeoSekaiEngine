/**
 * @file RenderProperties.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The class contains all the properties for rendering
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_RENDER_PROPERTIES_H_
#define _SEKAI_ENGINE_RENDER_RENDER_PROPERTIES_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Render/Color.h"

namespace SekaiEngine
{
    namespace Render
    {
        class EXTENDAPI RenderProperties
        {
        public:
            RenderProperties();
            RenderProperties(const RenderProperties& props);
            RenderProperties& operator=(const RenderProperties& props);
            virtual ~RenderProperties();

            Math::Vector2D Offset;
            Math::Vector2D Scale;
            Render::Color Tint;
            Math::Vector2D Origin;
            float Rotation;
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_PROPERTIES_H_