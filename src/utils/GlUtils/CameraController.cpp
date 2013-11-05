// CameraController.cpp

#include "CameraController.hpp"

#include "Camera.hpp"
using GlUtils::Camera;

#include <GLFW/glfw3.h>

namespace GlUtils {

//----------------------------------------------------------------------------------------
CameraController::CameraController()
    : cameraPtr(nullptr),
      cameraIsValid(false) {

}

//----------------------------------------------------------------------------------------
CameraController::~CameraController() {

}

//----------------------------------------------------------------------------------------
void CameraController::registerCamera(Camera * camera) {
    if ( (camera != 0) && (camera != nullptr) ) {
        this->cameraPtr = camera;
        cameraIsValid = true;
    } else {
        cameraIsValid = false;
    }
}

//----------------------------------------------------------------------------------------
/**
 * Updates the known key states given by user input.
 *
 * @param key
 * @param action
 * @param mods
 */
void CameraController::keyInput(int key, int action, int mods) {
    if (!cameraIsValid) return;

    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
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
        }
    }
}

//----------------------------------------------------------------------------------------
/**
 * Moves and orients the registered camera based on the last known state of the
 * keyboard and mouse.
 */
void CameraController::updateCamera() {
    if (!cameraIsValid) return;

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

} // end namespace GlUtils
