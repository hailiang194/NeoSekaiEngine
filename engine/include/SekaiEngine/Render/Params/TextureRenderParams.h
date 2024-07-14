/**
 * @file TextureRenderParams.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the parameters for rendering texture
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_PARAMS_TEXTURE_RENDER_PARAMS_H_
#define _SEKAI_ENGINE_RENDER_PARAMS_TEXTURE_RENDER_PARAMS_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/Params/RenderParams.h"
#include "SekaiEngine/Render/Texture.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Shape/Rectangle.h"
#include "SekaiEngine/Render/Color.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief The paramters for rendering texture
         * 
         */
        class EXTENDAPI TextureRenderParams: public RenderParams
        {
        public:
            /**
             * @brief Construct a new Texture Render Params object
             * 
             * @param props the properties for rendering texture
             * @param texture the whole texture needs drawing
             * @param position the position to rendering the texture
             * @param atlasPostion atlas position of texture inside the whole texture, if you want to render the whole texture use FULL_RECTANGLE
             */
            TextureRenderParams(const Render::RenderProperties& props, const Texture& texture, 
                const Math::Vector2D& position = Math::Vector2D(),
                const Shape::Rectangle& atlasPostion = FULL_RECTANGLE
            );
            TextureRenderParams(const TextureRenderParams& params) = delete;
            TextureRenderParams& operator=(const TextureRenderParams& params) = delete;
            /**
             * @brief Destroy the Texture Render Params object
             * 
             */
            ~TextureRenderParams();

            /**
             * @brief Get the whole texture
             * 
             * @return const Render::Texture& the whole texture
             */
            const Render::Texture& Texture() const;

            /**
             * @brief Get the whole texture
             * 
             * @return const Render::Texture& the whole texture 
             */
            const Render::Texture& Texture();

            /**
             * @brief Get the tint of texture
             * 
             * @return const Render::Color& the tint of texture
             */
            const Render::Color& Tint() const;

            /**
             * @brief Get the tint of texture
             * 
             * @return const Render::Color& the tint of texture
             */            
            const Render::Color& Tint();

            /**
             * @brief Get the rectangle of altas position
             * 
             * @return const Shape::Rectangle& the rectangle
             */
            const Shape::Rectangle& Source() const;
            
            /**
             * @brief Get the rectangle of altas position
             * 
             * @return const Shape::Rectangle& the rectangle
             */
            const Shape::Rectangle& Source();

            /**
             * @brief Get the rendering postion and size of drawn texture
             * 
             * @return const Shape::Rectangle& the rectangle which includes those information
             */
            const Shape::Rectangle& Destination() const;
            
            /**
             * @brief Get the rendering postion and size of drawn texture
             * 
             * @return const Shape::Rectangle& the rectangle which includes those information
             */
            const Shape::Rectangle& Destination();

            /**
             * @brief Get the rotation origin
             * 
             * @return const Math::Vector2D& the rotation origin
             */
            const Math::Vector2D& Origin() const;

            /**
             * @brief Get the rotation origin
             * 
             * @return const Math::Vector2D& the rotation origin
             */            
            const Math::Vector2D& Origin();

            /**
             * @brief Get the rotation angle in degree
             * 
             * @return const float& the rotation angle
             */
            const float& Rotation() const;
            
            /**
             * @brief Get the rotation angle in degree
             * 
             * @return const float& the rotation angle
             */
            const float& Rotation();

        private:
            Shape::Rectangle m_source; /*!< the position in the atlas*/
            Shape::Rectangle m_dest; /*!< the position for rendering*/
            Render::Texture m_texture; /*!< the whole texture*/
        };

        inline const Render::Texture& TextureRenderParams::Texture() const
        {
            return m_texture;
        }

        inline const Render::Texture& TextureRenderParams::Texture()
        {
            return static_cast<const TextureRenderParams&>(*this).Texture();
        }

        inline const Render::Color& TextureRenderParams::Tint() const
        {
            return m_props.Tint;
        }

        inline const Render::Color& TextureRenderParams::Tint()
        {
            return static_cast<const TextureRenderParams&>(*this).Tint();
        }

        inline const Shape::Rectangle& TextureRenderParams::Source() const
        {
            return m_source;
        }

        inline const Shape::Rectangle& TextureRenderParams::Source()
        {
            return static_cast<const TextureRenderParams&>(*this).Source();
        }

        inline const Shape::Rectangle& TextureRenderParams::Destination() const
        {
            return m_dest;
        }

        inline const Shape::Rectangle& TextureRenderParams::Destination()
        {
            return static_cast<const TextureRenderParams&>(*this).Destination();
        }

        inline const Math::Vector2D& TextureRenderParams::Origin() const
        {
            return m_props.Origin;
        }

        inline const Math::Vector2D& TextureRenderParams::Origin()
        {
            return static_cast<const TextureRenderParams&>(*this).Origin();
        }

        inline const float& TextureRenderParams::Rotation() const
        {
            return m_props.Rotation;
        }

        inline const float& TextureRenderParams::Rotation()
        {
            return static_cast<const TextureRenderParams&>(*this).Rotation();
        }

    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_PARAMS_TEXTURE_RENDER_PARAMS_H_