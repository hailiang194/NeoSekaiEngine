/**
 * @file TextureUI.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief the UI object of texture
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_OBJECT_TEXTURE_UI_H_
#define _SEKAI_ENGINE_OBJECT_TEXTURE_UI_H_

#include "./UI.h"
#include "SekaiEngine/Render/Texture.h"

namespace SekaiEngine
{
    namespace Object
    {
        /**
         * @brief UI object for texture
         * 
         */
        class EXTENDAPI TextureUI: public UI
        {
        public:
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param texture the texture of UI
             * @param parent the pointer of parent UI
             * @param self the render properties of the object
             */
            TextureUI(const Render::Texture& texture, UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param ui copied object
             */
            TextureUI(const TextureUI& ui);

            /**
             * @brief Copied assignment operator
             * 
             * @param ui Copied object
             * @return TextureUI& the reference of the object itself
             */
            TextureUI& operator=(const TextureUI& ui);

            /**
             * @brief Destroy the Texture UI object
             * 
             */
            virtual ~TextureUI();

            /**
             * @brief Handle update
             * 
             * @param elipse the elipse time of last frame
             */
            virtual void OnUpdate(const Timestep& elipse) override;

            /**
             * @brief Handle render
             * 
             */
            virtual void OnRender() override;

            /**
             * @brief Handle event
             * 
             * @param event event need to be handled
             */
            virtual void OnEvent(Event::Event& event) override;

            /**
             * @brief Set the Position of the object
             * 
             * @param position new position
             */
            void SetPosition(const Math::Vector2D& position) override;

            /**
             * @brief Set the Scale of the object
             * 
             * @param scale new scale
             */
            void SetScale(const Math::Vector2D& scale) override;

            /**
             * @brief Set the Tint for object
             * 
             * @param tint new tint
             */
            void SetTint(const Render::Color& tint) override;

            /**
             * @brief Set the Origin rotation for object
             * 
             * @param origin new origin
             */
            void SetOrigin(const Math::Vector2D& origin) override;

            /**
             * @brief Set the Rotation in degree for object
             * 
             * @param rotation new rotation in degree
             */
            void SetRotation(const float& rotation) override;
        private:
            Render::Texture m_texture; /*!< texture of UI object*/
        };
    } // namespace Object
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_OBJECT_TEXTURE_UI_H_
