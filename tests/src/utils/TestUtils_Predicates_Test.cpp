#include "gtest/gtest.h"

#include "glm/glm.hpp"

#include <TestUtils.hpp>
using namespace TestUtils::predicates;

#include <boost/math/special_functions/next.hpp>
using boost::math::float_next;
using boost::math::float_prior;

namespace {  // limit class visibility to this file.

    class TestUtils_Predicates_Test: public ::testing::Test {
    protected:
        // Ran before each test.
        virtual void SetUp() {
        }

        // Ran after each test.
        virtual void TearDown() {
        }
    };

}

//////////////////////////////////////////////////////////////////////////////////////////
// Test vec3_eq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq1) {
    vec3 v;
    EXPECT_PRED2(vec3_eq, v, v);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq2) {
    float f = 123.001f;
    vec3 v1(f, f, f);
    vec3 v2(float_next(f), f, float_prior(f));
    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq3) {
    float a = 99.999f;
    vec3 v1(a, a, a);

    float b = float_next(a);
    b = float_next(b);
    b = float_next(b);
    b = float_next(b);
    vec3 v2(a, a, b);

    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq4) {
    float a = -0.002f;
    vec3 v1(a, a, a);

    float b = float_prior(a);
    b = float_prior(b);
    b = float_prior(b);
    b = float_prior(b);
    vec3 v2(a, b, a);

    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq5) {
    float a = 100.0f;
    vec3 v1(a, a, a);

    float b = float_prior(a);
    b = float_prior(b);
    b = float_prior(b);
    b = float_prior(b);

    float c = float_next(a);
    b = float_next(b);
    b = float_next(b);
    b = float_next(b);

    vec3 v2(c, b, c);

    EXPECT_PRED2(vec3_eq, v1, v2);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Test vec3_neq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq1) {
    float a = -2.000001f;
    float b = float_next(a);
    for(int i = 0; i < 5; i++) {
        b = float_next(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(a, b, b);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq2) {
    float a = -2.000001f;
    float b = float_next(a);
    for(int i = 0; i < 5; i++) {
        b = float_next(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(b, b, a);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq3) {
    float a = -2.000001f;
    float b = float_next(a);
    for(int i = 0; i < 5; i++) {
        b = float_next(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(a, a, b);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq4) {
    float a = -2.000001f;
    float b = float_prior(a);
    for(int i = 0; i < 5; i++) {
        b = float_prior(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(a, b, b);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq5) {
    float a = 777777.0f;
    float b = float_prior(a);
    for(int i = 0; i < 5; i++) {
        b = float_prior(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(b, b, a);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq6) {
    float a = 1.00001f;
    float b = float_prior(a);
    for(int i = 0; i < 5; i++) {
        b = float_prior(b);
    }
    vec3 v1(a, a, a);
    vec3 v2(a, a, b);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test mat3_eq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq1) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 3; i++) {
        b = float_prior(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   a, a, b,
                   a, b, a);

    EXPECT_PRED2(mat3_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq2) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 3; i++) {
        b = float_prior(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   b, a, a,
                   a, a, a);

    EXPECT_PRED2(mat3_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq3) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 3; i++) {
        b = float_next(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   a, a, a,
                   a, a, b);

    EXPECT_PRED2(mat3_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq4) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 3; i++) {
        b = float_next(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   b, a, b,
                   a, a, a);

    EXPECT_PRED2(mat3_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq5) {
    float a = 3.0f;
    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);
    EXPECT_PRED2(mat3_eq, m1, m1);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test mat4_neq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_neq1) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 4; i++) {
        b = float_prior(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   a, a, b,
                   a, b, a);

    EXPECT_PRED2(mat3_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_neq2) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 4; i++) {
        b = float_prior(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   b, a, a,
                   a, a, a);

    EXPECT_PRED2(mat3_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_neq3) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 4; i++) {
        b = float_next(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   a, a, a,
                   a, a, b);

    EXPECT_PRED2(mat3_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_neq4) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 4; i++) {
        b = float_next(b);
    }

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   b, a, b,
                   a, a, a);

    EXPECT_PRED2(mat3_neq, m1, m2);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Test mat4_eq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_eq1) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 3; i++) {
        b = float_prior(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, a, a, a,
                   a, a, b, a,
                   a, a, a, a,
                   a, b, a, a);

    EXPECT_PRED2(mat4_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_eq2) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 3; i++) {
        b = float_prior(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, a, a, a,
                   b, a, a, a,
                   a, a, a, a,
                   a, a, a, b);

    EXPECT_PRED2(mat4_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_eq3) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 3; i++) {
        b = float_next(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, b, a, b,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    EXPECT_PRED2(mat4_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_eq4) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 3; i++) {
        b = float_next(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(b, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    EXPECT_PRED2(mat4_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_eq5) {
    float a = 3.0f;
    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);
    EXPECT_PRED2(mat4_eq, m1, m1);
}



//////////////////////////////////////////////////////////////////////////////////////////
// Test mat4_neq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_neq1) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 4; i++) {
        b = float_prior(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, a, a, a,
                   a, a, b, a,
                   a, a, a, a,
                   a, b, a, a);

    EXPECT_PRED2(mat4_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_neq2) {
    float a = 123.4567f;
    float b = float_prior(a);
    for(int i = 0; i < 4; i++) {
        b = float_prior(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, a, a, a,
                   b, a, a, a,
                   a, a, a, a,
                   a, a, a, b);

    EXPECT_PRED2(mat4_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_neq3) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 4; i++) {
        b = float_next(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, b, a, b,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    EXPECT_PRED2(mat4_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_neq4) {
    float a = 123.4567f;
    float b = float_next(a);
    for(int i = 0; i < 4; i++) {
        b = float_next(b);
    }

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(b, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    EXPECT_PRED2(mat4_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat4_neq5) {
    float a = 3.0f;
    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);
    mat4 m2 = m1;
    m2[2][3] += 0.0001f;
    EXPECT_PRED2(mat4_neq, m1, m2);
}
