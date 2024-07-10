/**
 * @file Layer.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Layer class
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_LAYER_LAYER_H_
#define _SEKAI_ENGINE_LAYER_LAYER_H_

#include <string>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Event/Event.h"
#include "SekaiEngine/Timer.h"

namespace SekaiEngine
{
    namespace Layer
    {
        class EXTENDAPI Layer
        {
        public:
            Layer(const char* name = "Layer");
            Layer(const Layer& layer);
            Layer& operator=(const Layer& layer);
            virtual ~Layer();

            virtual void OnAttach();
            virtual void OnDetach();
            virtual void OnUpdate(const Timestep& elipse);
            virtual void OnRender();
            virtual void OnEvent(Event::Event& event);

            const char* name() const;
            const char* name();
        protected:
            const char* m_debugName;
        };

        inline const char* Layer::name() const
        {
            return m_debugName;
        }

        inline const char* Layer::name()
        {
            return static_cast<const Layer&>(*this).name();
        }
    } // namespace Layer
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_LAYER_LAYER_H_