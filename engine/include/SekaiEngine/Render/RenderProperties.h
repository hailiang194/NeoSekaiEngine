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
        /**
         * @brief Render properties
         * 
         */
        class EXTENDAPI RenderProperties
        {
        public:
            /**
             * @brief Construct a new Render Properties object
             * 
             */
            RenderProperties();

            /**
             * @brief Construct a new Render Properties object
             * 
             * @param props copied object
             */
            RenderProperties(const RenderProperties& props);

            /**
             * @brief Copied assignment operator
             * 
             * @param props copied object
             * @return RenderProperties& the reference of the object itself
             */
            RenderProperties& operator=(const RenderProperties& props);

            /**
             * @brief Destroy the Render Properties object
             * 
             */
            virtual ~RenderProperties();

            Math::Vector2D Offset; /*!< Offset position*/
            Math::Vector2D Scale; /*!< Scale value*/
            Render::Color Tint; /*!< Tint color*/
            Math::Vector2D Origin; /*!< Rotation origin*/
            float Rotation; /*!< The rotation angle in degree*/
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_RENDER_PROPERTIES_H_