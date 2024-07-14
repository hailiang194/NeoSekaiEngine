/**
 * @file Shape.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Interface class for implementing shape
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SHAPE_SHAPE_H_
#define _SEKAI_ENGINE_SHAPE_SHAPE_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Shape
    {
        /**
         * @brief Shape interface
         * 
         */
        class EXTENDAPI IShape
        {
        public:
            /**
             * @brief Construct a new IShape object
             * 
             */
            IShape() = default;

            /**
             * @brief Construct a new IShape object
             * 
             * @param shape copied object
             */
            IShape(const IShape& shape) = default;

            /**
             * @brief Copied assignment operator
             * 
             * @param shape copied object
             * @return IShape& the reference of the object itself
             */
            IShape& operator=(const IShape& shape) = default;

            /**
             * @brief Destroy the IShape object
             * 
             */
            virtual ~IShape();
        };
    } // namespace Shape
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_SHAPE_SHAPE_H_