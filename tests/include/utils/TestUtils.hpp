#ifndef TESTUTILS_HPP_
#define TESTUTILS_HPP_

#include "gtest/gtest.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

namespace TestUtils {

    void ASSERT_ARRAY_EQ(const float * expected, const float * actual, size_t length) {
        for(size_t i = 0; i < length; i++) {
            ASSERT_FLOAT_EQ(expected[i], actual[i]);
        }
    }

    void ASSERT_VEC3_EQ(const vec3 & expected, const vec3 & actual) {
        ASSERT_FLOAT_EQ(expected.x, actual.x);
        ASSERT_FLOAT_EQ(expected.y, actual.y);
        ASSERT_FLOAT_EQ(expected.z, actual.z);
    }

    void ASSERT_VECTORS_EQ(const vector<vec3> & expected, const vector<vec3> & actual) {
        EXPECT_EQ(expected.size(), actual.size());

        for(size_t i = 0; i < expected.size(); i++) {
            ASSERT_VEC3_EQ(expected.at(i), actual.at(i));
        }
    }


}


#endif /* TESTUTILS_HPP_ */
