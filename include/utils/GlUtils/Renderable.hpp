/**
 * @brief Renderable
 *
 * @author Dustin Biser
 */

#ifndef RENDERABLE_HPP_
#define RENDERABLE_HPP_

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
using glm::vec3;
using glm::quat;
using glm::mat4;

#include "MaterialProperties.hpp"
#include "ModelTransform.hpp"

// Forward declarations
namespace GlUtils {
    struct BatchInfo;
    class ShaderProgram;
}

namespace GlUtils {

    /**
     * Datastructure for encapsulating the view and projection coordinate
     * transformation context for which a Renderable should be drawn.
     */
    struct RenderContext {
        mat4 viewMatrix;
        mat4 projectionMatrix;
    };

    class Renderable {
    public:
        Renderable(const GLuint * vao,
                   const ShaderProgram * shaderProgram,
                   const BatchInfo * batchInfo);

        ~Renderable() noexcept;

        void render(const RenderContext & context);

        void setShaderProgram(const ShaderProgram * shaderProgram);

        // Model Transform Operations
        void setPosition(const vec3 & position);
        void setPose(const quat & pose);
        void setScale(const vec3 & scale);

        // Material Properties
        void setEmissionLevels(const vec3 & emissionLevels);
        void setAmbientLevels(const vec3 & ambientLevels);
        void setDiffuseLevels(const vec3 & diffuseLevels);
        void setSpecularIntensity(float specular);
        void setShininessFactor(float shininessfactor);

    private:
        GLuint * vao;
        ShaderProgram * shaderProgram;
        BatchInfo * batchInfo;
        MaterialProperties material;
        ModelTransform modelTransform;

        void loadUniformData(const RenderContext & context);

    };
}

#endif /* RENDERABLE_HPP_ */
