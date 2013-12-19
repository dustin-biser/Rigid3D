/**
 * @brief Frustum
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_FRUSTUM_HPP_
#define RIGID3D_FRUSTUM_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    class Frustum {
    public:
        Frustum();

        Frustum(const Frustum & frustum);  // Copy Constructor

        Frustum(float left, float right, float bottom, float top, float zNear, float zFar);

        Frustum(float fieldOfViewY, float aspectRatio, float zNear, float zFar);

        virtual ~Frustum();

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
        float fovy;
        float aspectRatio;
        float zNear; // Distance to near z plane.
        float zFar;  // Distance to far z plane.
        bool _isPerspective;
        mutable bool recalcPerspectiveMatrix;

        mutable mat4 projectionMatrix;

        const static unsigned short floatTolerance = 5;
    };

} // end namespace GlUtils


#endif /* RIGID3D_FRUSTUM_HPP_ */
