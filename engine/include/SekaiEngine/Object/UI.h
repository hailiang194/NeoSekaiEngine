/**
 * @file UI.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Core UI Object class
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_OBJECT_UI_H_
#define _SEKAI_ENGINE_OBJECT_UI_H_

#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Render/RenderProperties.h"

namespace SekaiEngine
{
    namespace Object
    {
        /**
         * @brief UI core object
         * 
         */
        class EXTENDAPI UI
        {
        public:
            /**
             * @brief Construct a new UI object
             * 
             * @param parent parent UI object
             * @param self the properties of UI object
             */
            UI(UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            
            /**
             * @brief Construct a new UI object
             * 
             * @param ui Copied object
             */
            UI(const UI& ui);

            /**
             * @brief Copied assignment operator
             * 
             * @param ui Copied object
             * @return UI& the reference of the object itself
             */
            UI& operator=(const UI& ui);

            /**
             * @brief Destroy the UI object
             * 
             */
            virtual ~UI();

            /**
             * @brief Handle update
             * 
             * @param elipse the elipse time of last frame
             */
            virtual void OnUpdate(const Timestep& elipse) = 0;

            /**
             * @brief Handle render
             * 
             */
            virtual void OnRender();

            /**
             * @brief Handle event
             * 
             * @param event event need to be handled
             */
            virtual void OnEvent(Event::Event& event) = 0;

            /**
             * @brief Set the Position of the object
             * 
             * @param position new position
             */
            virtual void SetPosition(const Math::Vector2D& position);

            /**
             * @brief Set the Scale of the object
             * 
             * @param scale new scale
             */
            virtual void SetScale(const Math::Vector2D& scale);

            /**
             * @brief Get the pointer of parent UI
             * 
             * @return const UI* the poiner of parent UI
             */
            const UI* Parent() const;
            
            /**
             * @brief Get the pointer of parent UI
             * 
             * @return const UI* the poiner of parent UI
             */            
            const UI* Parent();

            /**
             * @brief Get the render properties of the object
             * 
             * @return const Render::RenderProperties& the reference of render properties
             */
            const Render::RenderProperties& Self() const;

            /**
             * @brief Get the render properties of the object
             * 
             * @return const Render::RenderProperties& the reference of render properties
             */
            const Render::RenderProperties& Self();

            /**
             * @brief Get the render properties for rendering
             * 
             * @return const Render::RenderProperties& the refernce of render properties
             */
            const Render::RenderProperties& Render() const;
            
            /**
             * @brief Get the render properties for rendering
             * 
             * @return const Render::RenderProperties& the refernce of render properties
             */
            const Render::RenderProperties& Render();
        protected:
            UI* m_parent; /*!< the pointer of parent UI*/
            Render::RenderProperties m_self; /*!< the properties of the object*/
            Render::RenderProperties m_render; /*!< the properties of rendering object */
        };

        inline const UI* UI::Parent() const
        {
            return m_parent;
        }


        inline const UI* UI::Parent()
        {
            return static_cast<const UI&>(*this).Parent();
        }

        inline const Render::RenderProperties& UI::Self() const
        {
            return m_self;
        }

        inline const Render::RenderProperties& UI::Self()
        {
            return static_cast<const UI&>(*this).Self();
        }

        inline const Render::RenderProperties& UI::Render() const
        {
            return m_render;
        }

        inline const Render::RenderProperties& UI::Render()
        {
            return static_cast<const UI&>(*this).Render();
        }
    } // namespace Object
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_OBJECT_UI_H_