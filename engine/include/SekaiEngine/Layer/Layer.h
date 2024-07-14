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
        /**
         * @brief Game layer
         * 
         */
        class EXTENDAPI Layer
        {
        public:
            /**
             * @brief Construct a new Layer object
             * 
             * @param name name of layer
             */
            Layer(const char* name = "Layer");

            /**
             * @brief Construct a new Layer object
             * 
             * @param layer copied layer
             */
            Layer(const Layer& layer);

            /**
             * @brief Copied assignment operator
             * 
             * @param layer copied layer
             * @return Layer& the reference of itself
             */
            Layer& operator=(const Layer& layer);

            /**
             * @brief Destroy the Layer object
             * 
             */
            virtual ~Layer();

            /**
             * @brief Handle when the layer is added to the stack
             * 
             */
            virtual void OnAttach();

            /**
             * @brief Handle when the layer is removed from the stack
             * 
             */
            virtual void OnDetach();

            /**
             * @brief Handle a loop
             * 
             * @param elipse the elipse time of last loop
             */
            virtual void OnUpdate(const Timestep& elipse);

            /**
             * @brief Handle render layer
             * 
             */
            virtual void OnRender();

            /**
             * @brief Handle events
             * 
             * @param event event which needs handling
             */
            virtual void OnEvent(Event::Event& event);

            /**
             * @brief get the name of layer
             * 
             * @return const char* the name of layer
             */
            const char* name() const;

            /**
             * @brief get the name of layer
             * 
             * @return const char* the name of layer
             */
            const char* name();
        protected:
            const char* m_debugName; /*!< name of layer*/
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