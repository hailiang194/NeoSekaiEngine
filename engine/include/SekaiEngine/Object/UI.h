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
        class UI
        {
        public:
            UI(UI* parent = nullptr, Render::RenderProperties self = Render::RenderProperties());
            UI(const UI& ui);
            UI& operator=(const UI& ui);
            virtual ~UI();

            virtual void OnUpdate(const Timestep& elipse) = 0;
            virtual void OnRender();
            virtual void OnEvent(Event::Event& event) = 0;

            const UI* Parent() const;
            const UI* Parent();

            const Render::RenderProperties& Self() const;
            Render::RenderProperties& Self();

            const Render::RenderProperties& Render() const;
            const Render::RenderProperties& Render();
        protected:
            UI* m_parent;
            Render::RenderProperties m_self;
            Render::RenderProperties m_render;
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

        inline Render::RenderProperties& UI::Self()
        {
            return const_cast<Render::RenderProperties&>(static_cast<const UI&>(*this).Self());
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