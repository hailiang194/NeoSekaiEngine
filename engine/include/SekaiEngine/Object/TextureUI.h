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



        private:
            Render::Texture m_texture; /*!< texture of UI object*/
        };
    } // namespace Object
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_OBJECT_TEXTURE_UI_H_
