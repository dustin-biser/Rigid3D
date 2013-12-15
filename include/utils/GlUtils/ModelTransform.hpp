/**
 * @brief ModelTransform
 *
 * @author Dustin Biser
 */

#ifndef MODELTRANSFORM_HPP_
#define MODELTRANSFORM_HPP_

#include "glm/glm.hpp"
using glm::vec3;
using glm::mat4;

#include "glm/gtc/quaternion.hpp"
using glm::quat;

namespace GlUtils {

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



#endif /* MODELTRANSFORM_HPP_ */
