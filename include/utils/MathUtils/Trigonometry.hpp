/**
 * @brief Trigonometry.hpp
 *
 * @author Dustin Biser
 */

#ifndef TRIGONOMETRY_HPP_
#define TRIGONOMETRY_HPP_

#include <math.h>

using std::tan;
using std::sin;
using std::cos;

namespace MathUtils {

    constexpr float PI = 3.1415926;

    //-----------------------------------------------------------------------------------
    inline float cotangent(float radians) {
        return (float) (1.0 / tan(radians));
    }

    //-----------------------------------------------------------------------------------
    inline float cosecant(float radians) {
        return (float) (1.0 / sin(radians));
    }

    //-----------------------------------------------------------------------------------
    inline float secant(float radians) {
        return (float) (1.0 / cos(radians));
    }

    //-----------------------------------------------------------------------------------
    inline float degreesToRadians(float degrees) {
        return PI * (degrees / 180.0f);
    }

    //-----------------------------------------------------------------------------------
    inline float radiansToDegrees(float radians) {
        return 180.0f * (radians / PI);
    }

}

#endif /* TRIGONOMETRY_HPP_ */
