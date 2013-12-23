#ifndef RIGID3D_SETTINGS_HPP_
#define RIGID3D_SETTINGS_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS

#include <cstdint>

namespace Rigid3D {

typedef std::int8_t byte;
typedef std::uint8_t ubyte;

typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;

typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;

typedef float float32;
typedef double float64;

// GLM types
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::uvec4 uvec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

typedef glm::quat quat;


// For computing hash codes from Rigid3D numeric types.
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

}

#endif /* RIGID3D_SETTINGS_HPP_ */
