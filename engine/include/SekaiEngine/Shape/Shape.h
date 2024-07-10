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
        class EXTENDAPI IShape
        {
        public:
            IShape() = default;
            IShape(const IShape& shape) = default;
            IShape& operator=(const IShape& shape) = default;
            virtual ~IShape();
        };
    } // namespace Shape
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_SHAPE_SHAPE_H_