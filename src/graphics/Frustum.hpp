/**
 * @brief Frustum
 *
 * @author Dustin Biser
 */

#ifndef FRUSTUM_HPP_
#define FRUSTUM_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rigid3D {

    class Frustum {
    public:
        Frustum();

        Frustum(const Frustum & frustum);  // Copy Constructor

        Frustum(float left, float right, float bottom, float top, float zNear, float zFar);

        Frustum(float fieldOfViewY, float aspectRatio, float zNear, float zFar);

        virtual ~Frustum();

        // Getters
        glm::mat4 getProjectionMatrix() const;
        float getFieldOfViewY() const;
        float getAspectRatio() const;
        float getNearZDistance() const;
        float getFarZDistance() const;

        // Setters
        void setFieldOfViewY(float fieldOfViewY);
        void setAspectRatio(float aspectRatio);
        void setNearZDistance(float zNear);
        void setFarZDistance(float zFar);
        void setProjectionMatrix(const glm::mat4 & projectionMatrix);

        bool isPerspective() const;
        bool isOrthographic() const;

    protected:
        float fovy;
        float aspectRatio;
        float zNear; // Distance to near z plane.
        float zFar;  // Distance to far z plane.
        bool _isPerspective;
        mutable bool recalcPerspectiveMatrix;

        mutable glm::mat4 projectionMatrix;

        const static unsigned short floatTolerance = 5;
    };

} // end namespace GlUtils


#endif /* FRUSTUM_HPP_ */
