#ifndef RIGID3D_SETTINGS_HPP_
#define RIGID3D_SETTINGS_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#define GLM_FORCE_RADIANS

#include <cstdint>
#include <cfloat>

namespace Rigid3D {

// Numeric Types /////////////////////////////////////////////////////////////////////////
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

// Vector Types //////////////////////////////////////////////////////////////////////////
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


// Constants /////////////////////////////////////////////////////////////////////////////
const float PI = 3.1415926f;
const float FLOAT_MIN = FLT_MIN;
const float FLOAT_MAX = FLT_MIN;
const float EPSILON = FLT_EPSILON;

}

#include "Hasher.hpp"

#endif /* RIGID3D_SETTINGS_HPP_ */
