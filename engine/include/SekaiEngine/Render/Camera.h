/**
 * @file Camera.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Game engine camera
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_CAMERA_H_
#define _SEKAI_ENGINE_RENDER_CAMERA_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Camera for 2D world
         * 
         */
        class EXTENDAPI Camera2D
        {
        public:
            /**
             * @brief Construct a new Camera 2D object
             * 
             * @param postion initialized position
             * @param offset initialized offset
             * @param zoom initialized zoom value
             * @param rotation initialized rotation in degree
             */
            Camera2D(
                const Math::Vector2D& postion = Math::Vector2D(), 
                const Math::Vector2D& offset = Math::Vector2D(), 
                const float& zoom = 1.0f,
                const float& rotation = 0.0f
            );

            /**
             * @brief Construct a new Camera 2D object
             * 
             * @param camera copied object
             */
            Camera2D(const Camera2D& camera);

            /**
             * @brief Copied assignement operator
             * 
             * @param camera copied object
             * @return Camera2D& the reference of itself
             */
            Camera2D& operator=(const Camera2D& camera);

            /**
             * @brief Destroy the Camera 2D object
             * 
             */
            virtual ~Camera2D();

            /**
             * @brief Start apply camera
             * 
             */
            void applyCamera();

            /**
             * @brief Finish apply camera
             * 
             */
            void unapplyCamera();

            Math::Vector2D Position; /*!< Camera position*/
            Math::Vector2D Offset; /*!< Camera offset*/
            float Zoom; /*!< Camera zoom value*/
            float Rotation; /*!< Camera rotation in degree*/
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_CAMERA_H_