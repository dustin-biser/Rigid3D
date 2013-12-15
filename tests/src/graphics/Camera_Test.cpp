// Camera_Test.cpp

#include "gtest/gtest.h"

#include <TestUtils.hpp>
using namespace TestUtils::predicates;

#include "Trigonometry.hpp"
using Rigid3D::PI;

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::dot;
using glm::mat4;
using glm::determinant;
using glm::transpose;

#include <glm/gtx/norm.hpp>
using glm::length;

#include "Camera.hpp"
using Rigid3D::Camera;

namespace {  // limit class visibility to this file.

    class Camera_Test : public ::testing::Test {
    protected:
        Camera camera;

        static const vec3 left_default;
        static const vec3 up_default;
        static const vec3 forward_default;
        static const float epsilon;

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

        void expect_orthonormal_camera_vectors() {
            vec3 l = camera.getLeftDirection();
            vec3 u = camera.getUpDirection();
            vec3 f = camera.getForwardDirection();

            EXPECT_NEAR(0.0f, dot(l, u), epsilon);
            EXPECT_NEAR(0.0f, dot(l, f), epsilon);
            EXPECT_NEAR(0.0f, dot(f, u), epsilon);

            EXPECT_NEAR(1.0f, length(l), epsilon);
            EXPECT_NEAR(1.0f, length(f), epsilon);
            EXPECT_NEAR(1.0f, length(u), epsilon);
        }

        void expect_orthonormal_view_matrix() {
            mat4 viewMatrix = camera.getViewMatrix();
            mat4 vTv = viewMatrix * transpose(viewMatrix);

            EXPECT_NEAR(1.0f, determinant(viewMatrix), epsilon);

            EXPECT_NEAR(1.0f, vTv[0][0], epsilon);
            EXPECT_NEAR(1.0f, vTv[1][1], epsilon);
            EXPECT_NEAR(1.0f, vTv[2][2], epsilon);
            EXPECT_NEAR(1.0f, vTv[3][3], epsilon);

            EXPECT_NEAR(1.0f, length(vTv[0]), epsilon);
            EXPECT_NEAR(1.0f, length(vTv[1]), epsilon);
            EXPECT_NEAR(1.0f, length(vTv[2]), epsilon);
            EXPECT_NEAR(1.0f, length(vTv[3]), epsilon);
        }

    };

    const vec3 Camera_Test::left_default = vec3(-1.0f, 0.0f, 0.0f);
    const vec3 Camera_Test::up_default = vec3(0.0f, 1.0f, 0.0f);
    const vec3 Camera_Test::forward_default = vec3(0.0f, 0.0f, -1.0f);
    const float Camera_Test::epsilon = 1.0e-6;
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
//////////////////////////////////////////////////////////////////////////////////////////
// Test lookAt
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @note The computed 3x3 upper left of the view matrix after calling lookAt
 * should equal:
 * v = |x0 x1 x2|
 *     |y0 y1 y2|
 *     |z0 z1 z2|
 * where -l=(x0, x1, x2), u=(y0, y1, y2), -f=(z0, z1, z2) are the camera basis vectors
 * given in world coordinates.  The columns of the computed view matrix thus describes
 * the world basis vectors using camera coordinates.
 */
//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_rightOfOrigin) {
    vec3 eye(0.0f, 0.0f, 0.0f);
    vec3 center(1.0f, 0.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    camera.lookAt(eye, center, up);

    mat3 m(0.0f, 0.0f, -1.0f,  // first column
           0.0f, 1.0f,  0.0f,  // second column
           1.0f, 0.0f,  0.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_rightOfOrigin2) {
    vec3 center(1.0f, 0.0f, 0.0f);
    camera.lookAt(center);

    mat3 m(0.0f, 0.0f, -1.0f,  // first column
           0.0f, 1.0f,  0.0f,  // second column
           1.0f, 0.0f,  0.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_leftOfOrigin) {
    vec3 eye(0.0f, 0.0f, 0.0f);
    vec3 center(-1.0f, 0.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    camera.lookAt(eye, center, up);

    mat3 m(0.0f, 0.0f, 1.0f,  // first column
           0.0f, 1.0f,  0.0f,  // second column
           -1.0f, 0.0f,  0.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_leftOfOrigin2) {
    vec3 center(-1.0f, 0.0f, 0.0f);
    camera.lookAt(center);

    mat3 m(0.0f, 0.0f, 1.0f,  // first column
           0.0f, 1.0f,  0.0f,  // second column
           -1.0f, 0.0f,  0.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_forward) {
    vec3 eye(0.0f, 0.0f, 0.0f);
    vec3 center(0.0f, 0.0f, -1.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    camera.lookAt(eye, center, up);

    EXPECT_PRED2(mat4_eq, mat4(), camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_forward2) {
    vec3 center(0.0f, 0.0f, -1.0f);
    camera.lookAt(center);

    EXPECT_PRED2(mat4_eq, mat4(), camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_backward) {
    vec3 eye(0.0f, 0.0f, 0.0f);
    vec3 center(0.0f, 0.0f, 1.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    camera.lookAt(eye, center, up);

    mat3 m(-1.0f, 0.0f, 0.0f,   // first column
            0.0f, 1.0f, 0.0f,   // second column
            0.0f, 0.0f, -1.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_backward2) {
    vec3 center(0.0f, 0.0f, 1.0f);
    camera.lookAt(center);

    mat3 m(-1.0f, 0.0f, 0.0f,   // first column
            0.0f, 1.0f, 0.0f,   // second column
            0.0f, 0.0f, -1.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, test_lookAt_up) {
    vec3 eye(0.0f, 0.0f, 0.0f);
    vec3 center(0.0f, 1.0f, 0.0f);
    vec3 up(0.0f, 0.0f, 1.0f);
    camera.lookAt(eye, center, up);

    mat3 m(1.0f, 0.0f, 0.0f,   // first column
           0.0f, 0.0f, -1.0f,   // second column
           0.0f, 1.0f, 0.0f); // third column
    mat4 viewMatrix(m);

    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
}

//----------------------------------------------------------------------------------------
//TEST_F(Camera_Test, test_lookAt_up2) {
//    vec3 center(0.0f, 1.0f, 0.0f);
//    camera.lookAt(center);
//
//    mat3 m(1.0f, 0.0f, 0.0f,   // first column
//           0.0f, 0.0f, -1.0f,   // second column
//           0.0f, 1.0f, 0.0f); // third column
//    mat4 viewMatrix(m);
//
//    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
//}
//
////----------------------------------------------------------------------------------------
//TEST_F(Camera_Test, test_lookAt_down) {
//    vec3 eye(0.0f, 0.0f, 0.0f);
//    vec3 center(0.0f, -1.0f, 0.0f);
//    vec3 up(0.0f, 0.0f, -1.0f);
//    camera.lookAt(eye, center, up);
//
//    mat3 m(1.0f, 0.0f, 0.0f,   // first column
//           0.0f, 0.0f, 1.0f,   // second column
//           0.0f, -1.0f, 0.0f); // third column
//    mat4 viewMatrix(m);
//
//    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
//}
//
////----------------------------------------------------------------------------------------
//TEST_F(Camera_Test, test_lookAt_down2) {
//    vec3 center(0.0f, -1.0f, 0.0f);
//    camera.lookAt(center);
//
//    mat3 m(1.0f, 0.0f, 0.0f,   // first column
//           0.0f, 0.0f, 1.0f,   // second column
//           0.0f, -1.0f, 0.0f); // third column
//    mat4 viewMatrix(m);
//
//    EXPECT_PRED2(mat4_eq, viewMatrix, camera.getViewMatrix());
//}

//////////////////////////////////////////////////////////////////////////////////////////
// Test roll, pitch, yaw
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, roll_does_not_modify_forward_vector) {
    const float radians = 0.1f;
    for(int i = 0; i < 60; i++) {
        camera.roll(radians);
    }
    expect_orthonormal_camera_vectors();
    expect_orthonormal_view_matrix();
    EXPECT_PRED2(vec3_eq, forward_default, camera.getForwardDirection());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, pitch_does_not_modify_left_vector) {
    const float radians = 0.1f;
    for(int i = 0; i < 60; i++) {
        camera.pitch(radians);
    }
    expect_orthonormal_camera_vectors();
    expect_orthonormal_view_matrix();
    EXPECT_PRED2(vec3_eq, left_default, camera.getLeftDirection());
}

//----------------------------------------------------------------------------------------
TEST_F(Camera_Test, yaw_does_not_modify_up_vector) {
    const float radians = 0.1f;
    for(int i = 0; i < 60; i++) {
        camera.yaw(radians);
    }
    expect_orthonormal_camera_vectors();
    expect_orthonormal_view_matrix();
    EXPECT_PRED2(vec3_eq, up_default, camera.getUpDirection());
}
