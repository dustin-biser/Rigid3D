/**
 * @brief ModelTransform
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_MODELTRANSFORM_HPP_
#define RIGID3D_MODELTRANSFORM_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    class ModelTransform {
    public:
        ModelTransform();
        ~ModelTransform();

        void setPosition(const vec3 & position);
        void setPose(const quat & pose);
        void setScale(const vec3 & scale);

        vec3 getPosition();
        quat getPose();
        vec3 getScale();
        mat4 getModelMatrix();

    private:
        bool recalcModelMatrix;
        vec3 position;
        quat pose;
        vec3 scaleFactor;
        mat4 modelMatrix;
    };

} // end namespace GlUtils

#endif /* RIGID3D_MODELTRANSFORM_HPP_ */
