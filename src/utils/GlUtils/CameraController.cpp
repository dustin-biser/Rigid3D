// CameraController.cpp

#include "CameraController.hpp"

#include "Camera.hpp"
using GlUtils::Camera;

#include <GLFW/glfw3.h>

#include <cassert>

#include "MathUtils.hpp"
using MathUtils::degreesToRadians;

// TODO (Dustin) Remove debug statements
#include <iostream>
using std::cout;
using std::endl;

namespace GlUtils {

//----------------------------------------------------------------------------------------
CameraController::CameraController()
    : cameraPtr(nullptr),
      flagRotation(false),
      xPos_prev(0),
      xPos(0),
      yPos_prev(0),
      yPos(0) {

}

//----------------------------------------------------------------------------------------
CameraController::~CameraController() {

}

//----------------------------------------------------------------------------------------
void CameraController::registerCamera(Camera * camera) {
    assert ( (camera != 0) && (camera != nullptr) );
    this->cameraPtr = camera;
}

//----------------------------------------------------------------------------------------
/**
 * Updates known key states input by the user.
 *
 * @param key
 * @param action
 * @param mods
 */
void CameraController::keyInput(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            key_r_down = true;
        } else if (key == GLFW_KEY_F) {
            key_f_down = true;
        } else if (key == GLFW_KEY_Q) {
            key_q_down = true;
        } else if (key == GLFW_KEY_E) {
            key_e_down = true;
        } else if (key == GLFW_KEY_W) {
            key_w_down = true;
        } else if (key == GLFW_KEY_S) {
            key_s_down = true;
        } else if (key == GLFW_KEY_A) {
            key_a_down = true;
        } else if (key == GLFW_KEY_D) {
            key_d_down = true;
        } else if (key == GLFW_KEY_LEFT_SHIFT) {
            key_left_shift_down = true;
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_R) {
            key_r_down = false;
        } else if (key == GLFW_KEY_F) {
            key_f_down = false;
        } else if (key == GLFW_KEY_Q) {
            key_q_down = false;
        } else if (key == GLFW_KEY_E) {
            key_e_down = false;
        } else if (key == GLFW_KEY_W) {
            key_w_down = false;
        } else if (key == GLFW_KEY_S) {
            key_s_down = false;
        } else if (key == GLFW_KEY_A) {
            key_a_down = false;
        } else if (key == GLFW_KEY_D) {
            key_d_down = false;
        } else if (key == GLFW_KEY_LEFT_SHIFT) {
            key_left_shift_down = false;
        }
    }
}

//----------------------------------------------------------------------------------------
/**
 * Updates mouse button key states input by the user.
 *
 * @param button
 * @param actions
 * @param mods
 */
void CameraController::mouseButtonInput(int button , int actions, int mods){

}

//----------------------------------------------------------------------------------------
/**
 * Updates mouse scroll states input by the user.
 *
 * @param xOffSet
 * @param yOffSet
 */
void CameraController::mouseScroll(double xOffSet, double yOffSet) {

}

//----------------------------------------------------------------------------------------
/**
 * Updates cursor position based on user input.
 *
 * @param xPos
 * @param yPos
 */
void CameraController::cursorPosition(double xPos, double yPos) {
    this->xPos_prev = this->xPos;
    this->xPos = xPos;

    this->yPos_prev = this->yPos;
    this->yPos = yPos;

    flagRotation = true;
}

//----------------------------------------------------------------------------------------
/**
 * Moves the registered camera based on the last known state of keyboard buttons,
 * mouse buttons, and cursor position.
 */
void CameraController::updateCamera() {
    assert ( (cameraPtr != 0) && (cameraPtr != nullptr) );
    updateTranslation();

    if (key_left_shift_down == false) {
        updateOrientation();
    }
}

//----------------------------------------------------------------------------------------
void CameraController::updateTranslation() {
    static const float translation_delta = 0.05f;

    if (key_r_down) {
        cameraPtr->translateRelative(0.0f, translation_delta, 0.0f);
    }
    if (key_f_down) {
        cameraPtr->translateRelative(0.0f, -1.0f * translation_delta, 0.0f);
    }
    if (key_a_down) {
        cameraPtr->translateRelative(translation_delta, 0.0f,  0.0f);
    }
    if (key_d_down) {
        cameraPtr->translateRelative(-1.0f * translation_delta, 0.0f,  0.0f);
    }
    if (key_w_down) {
        cameraPtr->translateRelative(0.0f, 0.0f, translation_delta);
    }
    if (key_s_down) {
        cameraPtr->translateRelative(0.0f, 0.0f, -1.0f * translation_delta);
    }
}

//----------------------------------------------------------------------------------------
void CameraController::updateOrientation() {
    if (flagRotation == false) { return; }

    static const float angleResolution = 0.05f;

    float xDelta = (float)(xPos - xPos_prev);
    float yDelta = (float)(yPos - yPos_prev);

    float radians;

    radians = degreesToRadians(-1.0f * angleResolution * xDelta);
    cameraPtr->yaw(radians);

    radians = degreesToRadians(-1.0f * angleResolution * yDelta);
    cameraPtr->pitch(radians);

    flagRotation = false;
}

} // end namespace GlUtils
