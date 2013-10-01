#include <GlmOutStream.hpp> // For printing glm types to output streams for failed tests.

#include "glm/glm.hpp"
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <vector>
using std::vector;

#include <boost/math/special_functions/next.hpp>
using boost::math::float_distance;

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

namespace TestUtils {
    namespace predicates {

    // Floating point numbers must be less than this distance apart to be considered equal.
    const int ulpTolerance = 5;

//    inline void ASSERT_ARRAY_EQ(const float * expected, const float * actual, size_t length) {
//        for(size_t i = 0; i < length; i++) {
//            ASSERT_FLOAT_EQ(expected[i], actual[i]);
//        }
//    }

    inline bool array_eq(const float * expected, const float * actual, size_t length) {
        for(size_t i = 0; i < length; i++) {
            if (!(std::abs((int)float_distance(expected[i], actual[i])) >= ulpTolerance)) {
                return false;
            }
        }

        return true;
    }

//    inline void ASSERT_VEC3_EQ(const vec3 & expected, const vec3 & actual) {
//        ASSERT_FLOAT_EQ(expected.x, actual.x);
//        ASSERT_FLOAT_EQ(expected.y, actual.y);
//        ASSERT_FLOAT_EQ(expected.z, actual.z);
//    }
//
//    inline void ASSERT_VEC4_EQ(const vec4 & expected, const vec4 & actual) {
//        ASSERT_FLOAT_EQ(expected.x, actual.x);
//        ASSERT_FLOAT_EQ(expected.y, actual.y);
//        ASSERT_FLOAT_EQ(expected.z, actual.z);
//        ASSERT_FLOAT_EQ(expected.w, actual.w);
//    }
//
//    inline void ASSERT_MAT3_EQ(const mat3 & expected, const mat3 & actual) {
//        ASSERT_VEC3_EQ(expected[0], actual[0]);
//        ASSERT_VEC3_EQ(expected[1], actual[1]);
//        ASSERT_VEC3_EQ(expected[2], actual[2]);
//    }
//
//    inline void ASSERT_MAT4_EQ(const mat4 & expected, const mat4 & actual) {
//        ASSERT_VEC4_EQ(expected[0], actual[0]);
//        ASSERT_VEC4_EQ(expected[1], actual[1]);
//        ASSERT_VEC4_EQ(expected[2], actual[2]);
//        ASSERT_VEC4_EQ(expected[3], actual[3]);
//    }

//    inline void ASSERT_VECTORS_EQ(const vector<vec3> & expected, const vector<vec3> & actual) {
//        EXPECT_EQ(expected.size(), actual.size());
//
//        for(size_t i = 0; i < expected.size(); i++) {
//            ASSERT_VEC3_EQ(expected.at(i), actual.at(i));
//        }
//    }


    //-----------------------------------------------------------------------------------
    inline bool vec3_eq(const vec3 & expected, const vec3 & actual) {
        return ( (std::abs((int)float_distance(expected.x, actual.x)) < ulpTolerance) &&
                 (std::abs((int)float_distance(expected.y, actual.y)) < ulpTolerance) &&
                 (std::abs((int)float_distance(expected.z, actual.z)) < ulpTolerance) );
    }

    //-----------------------------------------------------------------------------------
    inline bool vec3_neq(const vec3 & expected, const vec3 & actual) {
        return ( (std::abs((int)float_distance(expected.x, actual.x)) >= ulpTolerance) ||
                 (std::abs((int)float_distance(expected.y, actual.y)) >= ulpTolerance) ||
                 (std::abs((int)float_distance(expected.z, actual.z)) >= ulpTolerance) );
    }

    //-----------------------------------------------------------------------------------
    inline bool vec4_eq(const vec4 & expected, const vec4 & actual) {
        return ( (std::abs((int)float_distance(expected.x, actual.x)) < ulpTolerance) &&
                 (std::abs((int)float_distance(expected.y, actual.y)) < ulpTolerance) &&
                 (std::abs((int)float_distance(expected.z, actual.z)) < ulpTolerance) &&
                 (std::abs((int)float_distance(expected.w, actual.w)) < ulpTolerance) );
    }

    //-----------------------------------------------------------------------------------
    inline bool vec4_neq(const vec4 & expected, const vec4 & actual) {
        return ( (std::abs((int)float_distance(expected.x, actual.x)) >= ulpTolerance) ||
                 (std::abs((int)float_distance(expected.y, actual.y)) >= ulpTolerance) ||
                 (std::abs((int)float_distance(expected.z, actual.z)) >= ulpTolerance) ||
                 (std::abs((int)float_distance(expected.w, actual.w)) >= ulpTolerance) );
    }

    //-----------------------------------------------------------------------------------
    inline bool mat3_eq(const mat3 & expected, const mat3 & actual) {
        return (vec3_eq(expected[0], actual[0]) &&  // compare column one.
                vec3_eq(expected[1], actual[1]) &&  // compare column two.
                vec3_eq(expected[2], actual[2]) );  // compare column three.

    }

    //-----------------------------------------------------------------------------------
    inline bool mat3_neq(const mat3 & expected, const mat3 & actual) {
        return (vec3_neq(expected[0], actual[0]) ||  // compare column one.
                vec3_neq(expected[1], actual[1]) ||  // compare column two.
                vec3_neq(expected[2], actual[2]) );  // compare column three.

    }

    //-----------------------------------------------------------------------------------
    inline bool mat4_eq(const mat4 & expected, const mat4 & actual) {
        return (vec4_eq(expected[0], actual[0]) &&  // compare column one.
                vec4_eq(expected[1], actual[1]) &&  // compare column two.
                vec4_eq(expected[2], actual[2]) &&  // compare column three.
                vec4_eq(expected[3], actual[3]) );  // compare column four.

    }

    //-----------------------------------------------------------------------------------
    inline bool mat4_neq(const mat4 & expected, const mat4 & actual) {
        return (vec4_neq(expected[0], actual[0]) ||  // compare column one.
                vec4_neq(expected[1], actual[1]) ||  // compare column two.
                vec4_neq(expected[2], actual[2]) ||  // compare column three.
                vec4_neq(expected[3], actual[3]) );  // compare column four.

    }

    //-----------------------------------------------------------------------------------
    inline bool vectors_eq(const vector<vec3> & expected, const vector<vec3> & actual) {
        if (expected.size() != actual.size()) {
            return false;
        }

        for(size_t i = 0; i < expected.size(); i++) {
            if (!vec3_eq(expected.at(i), actual.at(i))) {
                return false;
            }
        }

        return true;
    }

}} // end namespace TestUtils::predicates
