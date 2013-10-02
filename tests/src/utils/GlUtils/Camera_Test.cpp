/**
 * @brief Camera_Test
 *
 * @author Dustin Biser
 */

#include "gtest/gtest.h"

#include <TestUtils.hpp>
using namespace TestUtils::predicates;

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;

#include <GlUtils/Camera.hpp>
using GlUtils::Camera;

namespace {  // limit class visibility to this file.

    class Camera_Test: public ::testing::Test {
    protected:
        Camera camera;

        static const vec3 left_default;
        static const vec3 up_default;
        static const vec3 forward_default;

        // Ran before each test.
        virtual void SetUp() {
            camera = Camera(45.0f, 1.0f, 1.0f, 10.0f);
        }

        // Ran after each test.
        virtual void TearDown() {
        }

        void expect_camera_vectors_were_not_modified() {
            EXPECT_PRED2(vec3_eq, left_default, camera.getLeftDirection());
            EXPECT_PRED2(vec3_eq, up_default, camera.getUpDirection());
            EXPECT_PRED2(vec3_eq, forward_default, camera.getForwardDirection());
        }

        void expect_view_matrix_was_not_modified() {
            EXPECT_PRED2(mat4_eq, mat4(), camera.getViewMatrix());
        }
    };

    const vec3 Camera_Test::left_default = vec3(-1.0f, 0.0f, 0.0f);
    const vec3 Camera_Test::up_default = vec3(0.0f, 1.0f, 0.0f);
    const vec3 Camera_Test::forward_default = vec3(0.0f, 0.0f, -1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test Default Values
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_default_position){
    EXPECT_PRED2(vec3_eq, vec3(0.0f, 0.0f, 0.0f), camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_default_leftDirection){
    EXPECT_PRED2(vec3_eq, left_default, camera.getLeftDirection());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_default_upDirection){
    EXPECT_PRED2(vec3_eq, up_default, camera.getUpDirection());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_default_forwardDirection){
    EXPECT_PRED2(vec3_eq, forward_default, camera.getForwardDirection());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_default_viewMatrix){
    mat4 expected;
    EXPECT_PRED2(mat4_eq, expected, camera.getViewMatrix());
}


//////////////////////////////////////////////////////////////////////////////////////////
// Test Translations
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_x){
    vec3 xDelta(2.0f, 0.0f, 0.0f);
    camera.translate(xDelta.x, xDelta.y, xDelta.z);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, xDelta, camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_xVec){
    vec3 xDelta(2.0f, 0.0f, 0.0f);
    camera.translate(xDelta);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, xDelta, camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_y){
    vec3 yDelta(0.0f, -10.0f, 0.0f);
    camera.translate(yDelta.x, yDelta.y, yDelta.z);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, yDelta, camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_yVec){
    vec3 yDelta(0.0f, -10.0f, 0.0f);
    camera.translate(yDelta);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, yDelta, camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_z){
    vec3 zDelta(0.0f, 0.0f, 99.0f);
    camera.translate(zDelta.x, zDelta.y, zDelta.z);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, zDelta, camera.getPosition());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_translate_zVec){
    vec3 zDelta(0.0f, 0.0f, 99.0f);
    camera.translate(zDelta);

    expect_camera_vectors_were_not_modified();
    EXPECT_PRED2(vec3_eq, zDelta, camera.getPosition());
}
