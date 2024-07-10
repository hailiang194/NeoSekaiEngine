/**
 * @file Utility.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All utility mathematic functions
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_MATH_UTILITY_H_
#define _SEKAI_ENGINE_MATH_UTILITY_H_

#include "SekaiEngine/BaseType.h"

#define DEFAULT_EPSILON 0.0001f

namespace SekaiEngine
{
    namespace Math
    {
        /// @brief Compare 2 float
        /// @param first first number
        /// @param second second number
        /// @param epsilon epsilon distance between 2 numbers to consider they are equal
        /// @return 1 if first > second, -1 if first < second and 0 if first == second
        EXTENDAPI const int cmpFloat(const float& first, const float& second, const float& epsilon = DEFAULT_EPSILON);

        EXTENDAPI const float DegreeToRadian(const float& degree);
    } // namespace Math
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_MATH_UTILITY_H_