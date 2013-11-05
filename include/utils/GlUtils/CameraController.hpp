/**
 * @brief CameraController
 *
 * @author Dustin Biser
 */

#ifndef CAMERA_CONTROLLER_HPP_
#define CAMERA_CONTROLLER_HPP_

namespace GlUtils { class Camera; }

namespace GlUtils {

    class CameraController {
    public:
        CameraController();
        ~CameraController();

        void registerCamera(Camera * camera);

        void keyInput(int key, int action, int mods);

        void updateCamera();

    private:
        Camera * cameraPtr;
        bool cameraIsValid;

        // Input Key States
        bool key_r_down = false;
        bool key_f_down = false;
        bool key_q_down = false;
        bool key_e_down = false;
        bool key_w_down = false;
        bool key_s_down = false;
        bool key_a_down = false;
        bool key_d_down = false;
    };

}

#endif /* CAMERA_CONTROLLER_HPP_ */
