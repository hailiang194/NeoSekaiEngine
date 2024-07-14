/**
 * @file LayerStack.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Layer stack
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_LAYER_LAYER_STACK_H_
#define _SEKAI_ENGINE_LAYER_LAYER_STACK_H_

#include <vector>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Layer/Layer.h"

namespace SekaiEngine
{
    namespace Layer
    {
        /**
         * @brief The stack of layers
         * 
         */
        class LayerStack 
        {
        public:
            /**
             * @brief Contruct a new Layer Stack object 
             */
            EXTENDAPI LayerStack();
            /**
             * @brief Construct a new Layer Stack object
             * 
             * @param stack copied stack
             */
            LayerStack(const LayerStack& stack);

            /**
             * @brief Copied assignment operator
             * 
             * @param stack copied stack
             * @return LayerStack& the reference of itself
             */
            LayerStack& operator=(const LayerStack& stack);

            /**
             * @brief Destroy the Layer Stack object
             * 
             */
            EXTENDAPI ~LayerStack();

            /**
             * @brief push the layer at the end of stack
             * 
             * @param layer pushed layer
             */
            EXTENDAPI void PushLayer(Layer* layer);

            /**
             * @brief push the layer at the first of stack
             * 
             * @param overlay pushed layer
             */
            EXTENDAPI void PushOverlay(Layer* overlay);

            /**
             * @brief pop the end layer of stack
             * 
             * @param layer the layer has been popped
             */
            EXTENDAPI void PopLayer(Layer* layer);

            /**
             * @brief pop the first layer of stack
             * 
             * @param overlay the layer has been popped
             */
            EXTENDAPI void PopOverlay(Layer* overlay);

            /**
             * @brief get the iterator of the beginning of stack
             * 
             * @return std::vector<Layer*>::iterator the iterator
             */
            std::vector<Layer*>::iterator begin();

            /**
             * @brief get the iterator of the end of stack
             * 
             * @return std::vector<Layer*>::iterator the iterator
             */
            std::vector<Layer*>::iterator end();
        private:
            std::vector<Layer*> m_layers; /*!< the layer stack*/
            std::vector<Layer*>::iterator m_layerInsert; /*!< the inserted layer iterator*/
        };

        inline std::vector<Layer*>::iterator LayerStack::begin()
        {
            return m_layers.begin();
        }

        inline std::vector<Layer*>::iterator LayerStack::end()
        {
            return m_layers.end();
        }
    } // namespace Layer
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_LAYER_LAYER_STACK_H_