#include "CameraController.hpp"

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Common/GlmOutStream.hpp>
#include <Rigid3D/Graphics/Camera.hpp>
#include <Rigid3D/Math/Trigonometry.hpp>

#include <GLFW/glfw3.h>

#include <glm/gtx/norm.hpp>

#include <iostream>

#include <cassert>

namespace Rigid3D {

using Rigid3D::Camera;
using Rigid3D::degreesToRadians;
using Rigid3D::PI;
using glm::dot;
using glm::length2;

//----------------------------------------------------------------------------------------
CameraController::CameraController()
    : camera(nullptr),
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
    this->camera = camera;
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
    static float fieldOfViewY = 45.0f;
    static const float delta = 5.0f;

    if (yOffSet < 0) {
       fieldOfViewY += delta;
    } else if (yOffSet > 0) {
       fieldOfViewY -= delta;
    }

    // Camp fieldOfViewY to [0, 180]
    if (fieldOfViewY < 0.0f) {
        fieldOfViewY = 0.0f;
    } else if (fieldOfViewY > 180.0f) {
        fieldOfViewY = 180.0f;
    }

    std::cout << "fieldOfViewY: " << fieldOfViewY << std::endl;
    camera->setFieldOfViewY(fieldOfViewY);
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
    assert ( (camera != 0) && (camera != nullptr) );

    updateTranslation();
    updatePose();

    if (key_left_shift_down == false) {
        updateLookAt();
    }
}

//----------------------------------------------------------------------------------------
/**
 * Reset the 'CameraController' object state, which includes all key and cursor position
 * states. The registered camera does not change.
 */
void CameraController::resetState() {
    xPos_prev = 0;
    xPos = 0;
    yPos_prev = 0;
    yPos = 0;
    key_r_down = false;
    key_f_down = false;
    key_q_down = false;
    key_e_down = false;
    key_w_down = false;
    key_s_down = false;
    key_a_down = false;
    key_d_down = false;
    key_left_shift_down = false;
    flagRotation = false;
}

//----------------------------------------------------------------------------------------
void CameraController::updateTranslation() {
    static const float translation_delta = 0.05f;

    if (key_r_down) {
        camera->translateRelative(0.0f, translation_delta, 0.0f);
    }
    if (key_f_down) {
        camera->translateRelative(0.0f, -1.0f * translation_delta, 0.0f);
    }
    if (key_a_down) {
        camera->translateRelative(translation_delta, 0.0f,  0.0f);
    }
    if (key_d_down) {
        camera->translateRelative(-1.0f * translation_delta, 0.0f,  0.0f);
    }
    if (key_w_down) {
        camera->translateRelative(0.0f, 0.0f, translation_delta);
    }
    if (key_s_down) {
        camera->translateRelative(0.0f, 0.0f, -1.0f * translation_delta);
    }
}

//----------------------------------------------------------------------------------------
void CameraController::updatePose() {
    static const float radians = 0.005f;
    if (key_q_down) {
        camera->roll(-1.0f * radians);
    }
    if (key_e_down) {
        camera->roll(radians);
    }
}

//----------------------------------------------------------------------------------------
/*
 * This method points the registered camera's forward vector towards a point on
 * an encompassing sphere, which the camera is at the center of.  This is what
 * gives the camera a first person "feel" based on mouse movements.
 */
void CameraController::updateLookAt() {
    if (flagRotation == false) { return; }
    if (xPos_prev == 0 || yPos_prev == 0) { return; }

    static const float angleDelta = (PI * 0.5f) * 0.0005f;
    static const float radius = 10.0f;
    static float polarAngle = PI * 0.5f;
    static float azimuthAngle = 0.0f;

    const float xDelta = (float)(xPos - xPos_prev);
    const float yDelta = (float)(yPos - yPos_prev);

    polarAngle += yDelta * angleDelta;
    azimuthAngle -= xDelta * angleDelta;

    // Prevent camera's forward vector from going completely vertical, so that
    // lookAt's up vector is never parallel with it.
    if (polarAngle < 0.001f) {
        polarAngle = 0.001f;
    }
    else if (polarAngle > PI) {
        polarAngle = PI - 0.001f;
    }

    if (azimuthAngle < 0.0f) {
        azimuthAngle += 2*PI;
    }
    else if (azimuthAngle > 2*PI) {
        azimuthAngle -= 2*PI;
    }

    const float sinPolar = sin(polarAngle);
    const float x = sinPolar * cos(azimuthAngle);
    const float y = sinPolar * sin(azimuthAngle);
    const float z = cos(polarAngle);

    // XXX(Dustin) - Refactor this method.  Need to take into consideration camera's initial center / forward direction.

    const vec3 f(0.0f, 0.0f, -1.0f);
    const vec3 u(0.0f, 1.0f, 0.0f);
    const vec3 l(-1.0f, 0.0f, 0.0f);

    vec3 center = radius * ((x * f) + (y * l) + (z * u));
    vec3 eye = camera->getPosition();
    center += eye;

    camera->lookAt(eye, center, u);


    flagRotation = false;
}

} // end namespace GlUtils
