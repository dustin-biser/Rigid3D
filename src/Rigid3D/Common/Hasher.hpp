#ifndef RIGID3D_HASHER_HPP_
#define RIGID3D_HASHER_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    /**
     * For computing hash codes for Rigid3D numeric and vector types.
     */
    class Hasher {
    private:
        static const size_t start = 17;
        static const size_t prime = 31;

    public:
        // Use for byte, ubyte, int8, int16, int32, uint8, uint16, uint32, float32, float64.
        template <typename T>
        size_t operator () (const T & n) const {
            return (size_t)n;
        }

        size_t operator () (const vec2 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;

            return result;
        }

        size_t operator () (const vec3 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;

            return result;
        }

        size_t operator () (const vec4 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;
            result = prime * result + (size_t)v.w;

            return result;
        }

        size_t operator () (const uvec2 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;

            return result;
        }

        size_t operator () (const uvec3 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;

            return result;
        }

        size_t operator () (const uvec4 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;
            result = prime * result + (size_t)v.w;

            return result;
        }

        size_t operator () (const ivec2 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;

            return result;
        }

        size_t operator () (const ivec3 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;

            return result;
        }

        size_t operator () (const ivec4 & v) const {
            size_t result = start;
            result = prime * result + (size_t)v.x;
            result = prime * result + (size_t)v.y;
            result = prime * result + (size_t)v.z;
            result = prime * result + (size_t)v.w;

            return result;
        }


    };

} // end namespace Rigid3D

#endif /* RIGID3D_HASHER_HPP_ */
