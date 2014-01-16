/**
 * @brief CameraController
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_CAMERA_CONTROLLER_HPP_
#define RIGID3D_CAMERA_CONTROLLER_HPP_

// Forward Declaration
namespace Rigid3D {
    class Camera;
}

namespace Rigid3D {

    class CameraController {
    public:
        CameraController();
        ~CameraController();

        void registerCamera(Camera & camera);

        void keyInput(int key, int action, int mods);
        void mouseButtonInput(int button , int actions, int mods);
        void mouseScroll(double xOffSet, double yOffSet);
        void cursorPosition(double xPos, double yPos);
        void updateCamera();
        void resetState();

    private:
        Camera * camera;

        bool flagRotation;

        double xScreenPos_prev;
        double xScreenPos;
        double yScreenPos_prev;
        double yScreenPos;

        // Input Key States
        bool key_r_down = false;
        bool key_f_down = false;
        bool key_q_down = false;
        bool key_e_down = false;
        bool key_w_down = false;
        bool key_s_down = false;
        bool key_a_down = false;
        bool key_d_down = false;
        bool key_left_shift_down = false;

        void updateLookAt();
        void updateTranslation();
        void updatePose();
    };

}

#endif /* RIGID3D_CAMERA_CONTROLLER_HPP_ */
