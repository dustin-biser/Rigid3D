/*
 * Frustum.hpp
 *
 *  Created on: Jul 10, 2013
 *      Author: dustin
 */

#ifndef FRUSTUM_HPP_
#define FRUSTUM_HPP_

#include <glm/gtc/matrix_transform.hpp>

namespace GlUtils {

    class Frustum {
    public:

        Frustum()
                : fieldOfViewY(45.0f),
                  aspectRatio(4.0f / 3.0f),
                  nearZPlane(1.0f),
                  farZPlane(100.0f) { }

        Frustum(float fieldOfViewY, float aspectRatio, float nearZPlane, float farZPlane)
                : fieldOfViewY(fieldOfViewY),
                  aspectRatio(aspectRatio),
                  nearZPlane(nearZPlane),
                  farZPlane(farZPlane) { }


        glm::mat4 getPerspectiveMatrix() {
            return glm::perspective(fieldOfViewY, aspectRatio, nearZPlane, farZPlane);
        }

        float getFieldOfViewY() {
            return fieldOfViewY;
        }

        float getAspectRatio() {
            return aspectRatio;
        }

        float getNearZPlaneDistance() {
            return nearZPlane;
        }

        float getFarZPlaneDistance() {
            return farZPlane;
        }

    private:
        float fieldOfViewY;
        float aspectRatio;
        float nearZPlane;
        float farZPlane;

    };

} // end namespace GlUtils


#endif /* FRUSTUM_HPP_ */
