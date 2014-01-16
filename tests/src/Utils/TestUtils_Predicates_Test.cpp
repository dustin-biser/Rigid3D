#include <gtest/gtest.h>

#include <Rigid3D/Rigid3D.hpp>
using namespace Rigid3D;

#include <TestUtils.hpp>
using namespace TestUtils;

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

        static const float e; // epsilon
    };

    const float TestUtils_Predicates_Test::e = Rigid3D::TestUtils::epsilon;

}
//////////////////////////////////////////////////////////////////////////////////////////
// Test float_eq_ulp
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_eq_ulp_1) {
    float a = 1e-4;
    float b = float_next(a);

    EXPECT_PRED2(float_eq_ulp, a, b);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_eq_ulp_2) {
    float a = 1e-4;
    float b = float_next(a);
    b = float_next(b);

    EXPECT_PRED2(float_eq_ulp, a, b);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_eq_ulp_3) {
    float a = 1e-4;
    float b = float_next(a);
    b = float_next(b);
    b = float_next(b);

    EXPECT_PRED2(float_eq_ulp, a, b);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_eq_ulp_4) {
    float a = 1234.5f;
    float b = float_next(a);
    b = float_next(b);
    b = float_next(b);
    b = float_next(b);

    EXPECT_PRED2(float_eq_ulp, a, b);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test float_eq_ulp
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_neq_ulp_1) {
    float a = 1e-4;
    float b = float_next(a);
    for (int i = 0; i < 4; i++) {
        b = float_next(b);
    }

    EXPECT_PRED2(float_neq_ulp, a, b);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_float_neq_ulp_2) {
    float a = 10.0f;
    float b = float_prior(a);
    for (int i = 0; i < 4; i++) {
        b = float_prior(b);
    }

    EXPECT_PRED2(float_neq_ulp, a, b);
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
    vec3 v2(f + e, f, f - e);
    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_eq3) {
    float a = 99.999f;
    vec3 v1(a, a, a);
    vec3 v2(a, a, a + 2 * e);

    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_smallFloats) {
    float a = -2.34e-10f;
    vec3 v1(a, a, a);
    vec3 v2(a, a - e, a);

    EXPECT_PRED2(vec3_eq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_largeFloats) {
    float a = 999999.9f;
    vec3 v1(a, a, a);
    vec3 v2(a + 2*e, a - e, a + 2*e);

    EXPECT_PRED2(vec3_eq, v1, v2);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Test vec3_neq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq1) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a, a + 10*e, a);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq2) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a, a, a + 10*e);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq3) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a + 10*e, a, a);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq4) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a, a - 8*e, a - 10*e);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq5) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a - 8*e, a + 8*e, a);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq6) {
    float a = -2.000001f;
    vec3 v1(a, a, a);
    vec3 v2(a - 8*e, a - 8*e, a - 8*e);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq_smallFloat) {
    float a = -2.34e-10f;
    vec3 v1(a, a, a);
    vec3 v2(a + 8*e, a, a - 8*e);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_vec3_neq_largeFloat) {
    float a = -99999.9f;
    vec3 v1(a, a, a);
    vec3 v2(a + 0.05f, a, a - 0.05f);

    EXPECT_PRED2(vec3_neq, v1, v2);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test mat3_eq
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq1) {
    float a = 123.456f;
    float b = a + 3*e;

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
    float a = -123.456f;
    float b = a + e;

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
    float a = -0.000003456f;
    float b = a + 1e-11;

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
    float a = -0.0000000123f;
    float b = a + 1e-11;

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   b, a, b,
                   a, a, a);

    EXPECT_PRED2(mat3_eq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_eq_self) {
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
    float a = -0.0000000123f;
    float b = a + 10*e;

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
    float a = 0.0000000123f;
    float b = a + 10*e;

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
    float a = 0.0000000123f;
    float b = a + 8*e;

    mat3 m1 = mat3(a, a, a,
                   a, a, a,
                   a, a, a);

    mat3 m2 = mat3(a, a, a,
                   a, a, a,
                   a, a, b);

    EXPECT_PRED2(mat3_neq, m1, m2);
}

//----------------------------------------------------------------------------------------
TEST_F(TestUtils_Predicates_Test, test_mat3_largeFloats) {
    float a = 34567.8f;
    float b = a + 0.05f;

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
    float b = a + 3*e;

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
    float b = a + 2*e;

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
    float a = 123.4e-7f;
    float b = a + 2e-13;

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
    float a = 123.4f;
    float b = a + 2*e;

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
TEST_F(TestUtils_Predicates_Test, test_mat4_eq_self) {
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
    float a = 1.0f;
    float b = a + 8*e;

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
    float a = -10.0f;
    float b = a + 30*e;

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
    float a = -100.0f;
    float b = a + 1.0e-3f;

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
TEST_F(TestUtils_Predicates_Test, test_mat4_neq_smallFloats) {
    float a = 1.0e-4f;
    float b = a + 1.0e-6f;

    mat4 m1 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, a, a, a);

    mat4 m2 = mat4(a, a, a, a,
                   a, a, a, a,
                   a, a, a, a,
                   a, b, a, a);

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
    m2[2][3] += 0.00001f;
    EXPECT_PRED2(mat4_neq, m1, m2);
}
