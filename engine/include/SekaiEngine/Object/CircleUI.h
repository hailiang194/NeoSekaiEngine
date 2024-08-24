/**
 * @file CircleUI.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief UI for circle
 * @version 0.1
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_OBJECT_CIRCLE_UI_H_
#define _SEKAI_ENGINE_OBJECT_CIRCLE_UI_H_

#include "UI.h"
#include "SekaiEngine/Shape/Circle.h"

namespace SekaiEngine
{
    namespace Object
    {
        /**
         * @brief UI object for circle
         * 
         */
        class EXTENDAPI CircleUI: public UI 
        {
        public:
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param circle the circle of UI
             * @param parent the pointer of parent UI
             * @param self the render properties of the object
             */
            CircleUI(const Shape::Circle& circle, UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            
            /**
             * @brief Construct a new Texture UI object
             * 
             * @param ui copied object
             */
            CircleUI(const CircleUI& ui);

            /**
             * @brief Copied assignment operator
             * 
             * @param ui Copied object
             * @return CircleUI& the reference of the object itself
             */
            CircleUI& operator=(const CircleUI& ui);

            /**
             * @brief Destroy the Texture UI object
             * 
             */
            virtual ~CircleUI();

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
        protected:
            Shape::Circle m_circle;
        };
    } // namespace Object
    
} // namespace SekaiEngine


#endif//!#define _SEKAI_ENGINE_OBJECT_CIRCLE_UI_H_