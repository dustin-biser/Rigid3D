// AABB_Test.cpp

#include "gtest/gtest.h"

#include <Rigid3D/Collision/AABB.hpp>
using Rigid3D::AABB;

#include <Rigid3D/Collision/RayCastInput.hpp>
#include <Rigid3D/Collision/RayCastOutput.hpp>
using Rigid3D::RayCastInput;
using Rigid3D::RayCastOutput;

#include "TestUtils.hpp"
using namespace TestUtils::predicates;

namespace {  // limit class visibility to this file.

    class AABB_Test : public ::testing::Test {
    protected:
        static AABB aabb;
        static const vec3 min_point;
        static const vec3 max_point;

        RayCastInput rayCastIn;
        RayCastOutput rayCastOut;

        // Ran before each test.
        virtual void SetUp() {
            // AABB is center at the origin with side length of 2.
            aabb.minBounds = min_point;
            aabb.maxBounds = max_point;
        }

    };

    AABB AABB_Test::aabb;
    const vec3 AABB_Test::min_point = vec3(-1.0f);
    const vec3 AABB_Test::max_point = vec3(1.0f);

}

//----------------------------------------------------------------------------------------
TEST_F(AABB_Test, ray_cast_does_not_modify_min_max) {
    rayCastIn.p1 = vec3(-10.0f, -10.0f, -10.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, min_point, aabb.minBounds);
    EXPECT_PRED2(vec3_eq, max_point, aabb.maxBounds);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast from outside AABB starting at point on the x-axis less than
 * aabb.min.x, with direction along the x-axis.
 */
TEST_F(AABB_Test, ray_cast_outside_aabb_along_x_axis) {
    rayCastIn.p1 = vec3(-10.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(-1.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 9.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast miss.  Start ray from outside AABB starting at point on the x-axis
 * less than aabb.min.x, with direction along the x-axis.
 */
TEST_F(AABB_Test, ray_cast_miss_outside_aabb_along_x_axis) {
    rayCastIn.p1 = vec3(-10.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 8.0f; // Length to short to hit.

    EXPECT_FALSE(aabb.rayCast(rayCastIn, &rayCastOut));
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast from outside AABB starting at point on the x-axis greater than
 * aabb.max.x, with direction along -x.
 */
TEST_F(AABB_Test, ray_cast_outside_aabb_along_x_axis2) {
    rayCastIn.p1 = vec3(10.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(1.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 9.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside the AABB going along the +x direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_x_axis) {
    rayCastIn.p1 = vec3(0.0f);
    rayCastIn.p2 = vec3(0.1f, 0.0f, 0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside the AABB going along the -x direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_x_axis2) {
    rayCastIn.p1 = vec3(0.0f);
    rayCastIn.p2 = vec3(-0.1f, 0.0f, 0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from outside AABB at point on y-axis less than aabb.min.y,
 * with direction along +y.
 */
TEST_F(AABB_Test, ray_cast_outside_aabb_along_y_axis) {
    rayCastIn.p1 = vec3(0.0f, -10.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, -1.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 9.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside AABB going along +y direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_y_axis) {
    rayCastIn.p1 = vec3(0.0f);
    rayCastIn.p2 = vec3(0.0f, 1.0f, 0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, rayCastIn.p1, rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside AABB going along -y direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_y_axis2) {
    rayCastIn.p1 = vec3(0.0f);
    rayCastIn.p2 = vec3(0.0f, -1.0f, 0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, rayCastIn.p1, rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from outside AABB at point on z-axis less than aabb.min.z,
 * with direction along +z.
 */
TEST_F(AABB_Test, ray_cast_outside_aabb_along_z_axis) {
    rayCastIn.p1 = vec3(0.0f, 0.0f, -10.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, -1.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 9.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from outside AABB at point on z-axis greater than aabb.max.z,
 * with direction along -z.
 */
TEST_F(AABB_Test, ray_cast_outside_aabb_along_z_axis2) {
    rayCastIn.p1 = vec3(0.0f, 0.0f, 10.0f);
    rayCastIn.p2 = vec3(0.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 1.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 9.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside AABB going along +z direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_z_axis) {
    rayCastIn.p1 = vec3(0.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f, 0.0f, 1.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast starting from inside AABB going along -z direction.
 */
TEST_F(AABB_Test, ray_cast_inside_aabb_along_z_axis2) {
    rayCastIn.p1 = vec3(0.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f, 0.0f, -1.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_TRUE(aabb.rayCast(rayCastIn, &rayCastOut));
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 0.0f), rayCastOut.hitPoint);
    EXPECT_PRED2(float_eq, 0.0f, rayCastOut.length);
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast miss. Intersects x and y slabs, but misses z slab.
 * Starting outside AABB at point less that aabb.min.z.
 */
TEST_F(AABB_Test, ray_cast_miss_z_hit_x_y) {
    rayCastIn.p1 = vec3(0.0f, 0.0f, -4.0f);
    rayCastIn.p2 = vec3(0.0f, 4.0f, -3.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_FALSE(aabb.rayCast(rayCastIn, &rayCastOut));
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast miss. Intersects z and y slabs, but misses x slab.
 */
TEST_F(AABB_Test, ray_cast_miss_x_hit_y_z) {
    rayCastIn.p1 = vec3(-4.0f, 0.0f, 0.0f);
    rayCastIn.p2 = vec3(-3.0f, 0.0f, -3.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_FALSE(aabb.rayCast(rayCastIn, &rayCastOut));
}

//----------------------------------------------------------------------------------------
/*
 * Ray cast miss. Intersects x and z slabs, but misses y slab.
 */
TEST_F(AABB_Test, ray_cast_miss_y_hit_x_z) {
    rayCastIn.p1 = vec3(0.0f, 5.0f, 0.0f);
    rayCastIn.p2 = vec3(0.0f, 4.0f, 1.0f);
    rayCastIn.maxLength = 100.0f;

    EXPECT_FALSE(aabb.rayCast(rayCastIn, &rayCastOut));
}
