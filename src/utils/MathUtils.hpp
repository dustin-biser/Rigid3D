/*
 * MathUtils.hpp
 *
 *  Created on: Jul 10, 2013
 *      Author: dustin
 */

#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

class MathUtils {
public:
    static constexpr float PI = 3.1415926;

    static inline float coTangent(float radians) {
        return  (float) (1 / tan(radians));
    }

    static inline float degreesToRadians(float degrees) {
        return PI * degrees / 180.0f;
    }
};


#endif /* MATHUTILS_HPP_ */
