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
        mat4 getProjectionMatrix() const;
        float getFieldOfViewY() const;
        float getAspectRatio() const;
        float getNearZDistance() const;
        float getFarZDistance() const;

        // Setters
        void setFieldOfViewY(float fieldOfViewY);
        void setAspectRatio(float aspectRatio);
        void setNearZDistance(float zNear);
        void setFarZDistance(float zFar);
        void setProjectionMatrix(const mat4 & projectionMatrix);

        bool isPerspective() const;
        bool isOrthographic() const;

    protected:
        float fieldOfViewY;
        float aspectRatio;
        float zNear;
        float zFar;
        bool _isPerspective;
        mutable bool recalcPerspectiveMatrix;

        mutable mat4 projectionMatrix;

        const static unsigned short floatTolerance = 5;
    };

} // end namespace GlUtils


#endif /* FRUSTUM_HPP_ */
