/**
 * @brief Frustum
 *
 * @author Dustin Biser
 */

#ifndef FRUSTUM_HPP_
#define FRUSTUM_HPP_

#include <glm/gtc/matrix_transform.hpp>

#include <glm/glm.hpp>
using glm::mat4;

namespace GlUtils {

    class Frustum {
    public:
        Frustum();

        Frustum(float left, float right, float bottom, float top, float zNear, float zFar);

        Frustum(float fieldOfViewY, float aspectRatio, float zNear, float zFar);

        // Getters
        glm::mat4 getProjectionMatrix();
        float getFieldOfViewY();
        float getAspectRatio();
        float getNearZDistance();
        float getFarZDistance();

        // Setters
        void setFieldOfViewY(float fieldOfViewY);
        void setAspectRatio(float aspectRatio);
        void setNearZDistance(float zNear);
        void setFarZDistance(float zFar);

        bool isPerspective();
        bool isOrthographic();

    private:
        float fieldOfViewY;
        float aspectRatio;
        float zNear;
        float zFar;
        bool _isPerspective;
        bool recalcPerspectiveMatrix;

        mat4 projectionMatrix;

        const static short floatTolerance = 5;
    };

} // end namespace GlUtils


#endif /* FRUSTUM_HPP_ */
