#include "CameraController.hpp"

#include <Rigid3D/Common/GlmOutStream.hpp>
#include <Rigid3D/Graphics/Camera.hpp>
#include <Rigid3D/Math/Trigonometry.hpp>

#include <GLFW/glfw3.h>

#include <glm/gtx/norm.hpp>


#include <cassert>


// TODO Dustin - Remove this:
    #include <iostream>
    #include <glm/gtx/io.hpp>
    using namespace std;

namespace Rigid3D {

using namespace glm;

//----------------------------------------------------------------------------------------
CameraController::CameraController()
    : camera(nullptr),
      xCursorPos_prev(0),
      xCursorPos(0),
      yCursorPos_prev(0),
      yCursorPos(0),
      flagRotation(false) {

    //-- Set initial transform scale factors to resonable defaults:
    float angle = PI * 0.5 * 0.001;
    scaleFactors.rollAngle = 10*angle;
    scaleFactors.pitchAngle = angle;
    scaleFactors.yawAngle = angle;
    scaleFactors.forwardDelta = 0.1f;
    scaleFactors.sideStrafeDelta = 0.1f;
    scaleFactors.upDelta = 0.1f;
}

//----------------------------------------------------------------------------------------
CameraController::~CameraController() {

}

//----------------------------------------------------------------------------------------
void CameraController::registerCamera(Camera * camera) {
    assert (camera != nullptr);
    this->camera = camera;

    yawAxis = camera->getUpDirection();
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
void CameraController::mouseButton(int button, int actions, int mods){

}

//----------------------------------------------------------------------------------------
/**
 * Updates mouse scroll states input by the user.
 *
 * @param xOffSet
 * @param yOffSet
 */
void CameraController::mouseScroll(double xOffSet, double yOffSet) {

    // TODO Dustin - Use the Camera's fieldOfViewY as starting value, don't hardcode!
    static float fieldOfViewY = 70.0f;
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
    xCursorPos_prev = xCursorPos;
    xCursorPos = xPos;

    yCursorPos_prev = yCursorPos;
    yCursorPos = yPos;

    flagRotation = true;
}

//----------------------------------------------------------------------------------------
/**
 * Moves the registered camera based on the last known state of keyboard buttons,
 * mouse buttons, and cursor position.
 */
void CameraController::updateCamera() {
    assert (camera != nullptr);

    updateTranslation();
    updateRoll();

    if (key_left_shift_down == false) {
        updateLookAt();
    }
}

//----------------------------------------------------------------------------------------
/**
 * Reset the 'CameraController' object state, which includes all key and cursor position
 * states. The registered camera does not change.
 */
void CameraController::reset() {
    xCursorPos_prev = 0;
    xCursorPos = 0;
    yCursorPos_prev = 0;
    yCursorPos = 0;
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
    if (key_r_down) {
        camera->translateLocal(0.0f, scaleFactors.upDelta, 0.0f);
    }
    if (key_f_down) {
        camera->translateLocal(0.0f, -scaleFactors.upDelta, 0.0f);
    }
    if (key_a_down) {
        camera->translateLocal(scaleFactors.sideStrafeDelta, 0.0f, 0.0f);
    }
    if (key_d_down) {
        camera->translateLocal(-scaleFactors.sideStrafeDelta, 0.0f, 0.0f);
    }
    if (key_w_down) {
        camera->translateLocal(0.0f, 0.0f, scaleFactors.forwardDelta);
    }
    if (key_s_down) {
        camera->translateLocal(0.0f, 0.0f, -scaleFactors.forwardDelta);
    }
}

//----------------------------------------------------------------------------------------
void CameraController::updateRoll() {
    float angle = scaleFactors.rollAngle;
    if (key_q_down) {
        angle *= -1;
        camera->roll(angle);
        rotateYawAxis(angle, camera->getForwardDirection());
    }
    if (key_e_down) {
        camera->roll(angle);
        rotateYawAxis(angle, camera->getForwardDirection());
    }

}

//----------------------------------------------------------------------------------------
void CameraController::rotateYawAxis(float angle, vec3 axis) {
    quat q = angleAxis(angle, axis);
    yawAxis = glm::rotate(q, yawAxis);
}

//----------------------------------------------------------------------------------------
void CameraController::updateLookAt() {
    if (flagRotation == false) { return; }
    if (xCursorPos_prev == 0 || yCursorPos_prev == 0) { return; }

    float xDelta = (float)(xCursorPos - xCursorPos_prev);
    float yDelta = (float)(yCursorPos - yCursorPos_prev);

    float pitchAngle = -yDelta * scaleFactors.pitchAngle;
    camera->pitch(pitchAngle);
    camera->rotate(-xDelta * scaleFactors.yawAngle, yawAxis);

    flagRotation = false;
}

//----------------------------------------------------------------------------------------
void CameraController::setRollScaleFactor(float rollScale) {
    scaleFactors.rollAngle = rollScale;
}

//----------------------------------------------------------------------------------------
void CameraController::setPitchScaleFactor(float pitchScale) {
    scaleFactors.pitchAngle = pitchScale;
}

//----------------------------------------------------------------------------------------
void CameraController::setYawScaleFactor(float yawScale) {
    scaleFactors.yawAngle = yawScale;
}

//----------------------------------------------------------------------------------------
void CameraController::setUpScaleFactor(float upScale) {
    scaleFactors.upDelta = upScale;
}

//----------------------------------------------------------------------------------------
void CameraController::setForwardScaleFactor(float forwardScale) {
    scaleFactors.forwardDelta = forwardScale;
}

//----------------------------------------------------------------------------------------
void CameraController::setSideStrafeScaleFactor(float sideStrafeScale) {
    scaleFactors.sideStrafeDelta = sideStrafeScale;
}

} // end namespace GlUtils
