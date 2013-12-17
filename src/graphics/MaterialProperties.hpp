/**
 * @brief MaterialProperties
 *
 * @author Dustin Biser
 */

#ifndef MATERIALPROPERTIES_HPP_
#define MATERIALPROPERTIES_HPP_

#include "glm/glm.hpp"

namespace Rigid3D {

    /**
     * Material properties for emission, ambient, diffuse, and specular lighting.
     */
    struct MaterialProperties {
        glm::vec3 emission; // Emission light intensity from material for each RGB component.
        glm::vec3 Ka;       // Coefficients of ambient reflectivity for each RGB component.
        glm::vec3 Kd;       // Coefficients of diffuse reflectivity for each RGB component.
        float Ks;           // Coefficient of specular reflectivity, uniform across each RGB component.
        float shininessFactor;  // Specular shininess factor.
    };

}


#endif /* MATERIALPROPERTIES_HPP_ */
