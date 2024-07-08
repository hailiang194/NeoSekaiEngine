#include "SekaiEngine/Math/Utility.h"
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

namespace SekaiEngine
{
    namespace Math
    {
        const int cmpFloat(const float& first, const float& second, const float& epsilon)
        {
            if (std::fabs(first - second) < epsilon)
                return 0;

            return (first > second) ? 1 : -1;
        }

        const float DegreeToRadian(const float& degree)
        {
            return degree * M_PI / 180.0f;
        }
    } // namespace Math
    
} // namespace SekaiEngine
