/**
 * @brief Renderable
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_RENDERABLE_HPP_
#define RIGID3D_RENDERABLE_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/MaterialProperties.hpp>
#include <Rigid3D/Graphics/ModelTransform.hpp>

#include <OpenGL/gltypes.h>

// Forward declarations
namespace Rigid3D {
    struct BatchInfo;
    class ShaderProgram;
}

namespace Rigid3D {

    /**
     * Datastructure for encapsulating the view and projection coordinate
     * transformation context for which a Renderable should be drawn.
     */
    struct RenderContext {
        mat4 viewMatrix;
        mat4 projectionMatrix;
    };

    /**
     * @brief Class for encapsulating the data required to render a mesh.
     *
     * This class holds information including:
     * # Model Transformations
     * # Material Lighting Properties
     * # ShaderProgram
     * # Vertex Array Object
     * # BatchInfo
     *
     * @note The following uniform variables must be present in the attached 'ShaderProgram'
     * for a 'Renderable' object to be rendered without error.
     *
     * # uniform mat4 ModelViewMatrix
     * # uniform mat4 ProjectionMatrix
     * # uniform mat3 NormalMatrix
     * # struct MaterialProperties {
     *      vec3 emission;
     *      vec3 Ka;
     *      vec3 Kd;
     *      float Ks;
     *      float shininessFactor;
     *   };
     *   uniform MaterialProperties material;
     */
    class Renderable {
    public:
        Renderable(const GLuint * vao,
                   const ShaderProgram * shaderProgram,
                   const BatchInfo * batchInfo);

        Renderable();

        ~Renderable();

        void render(const RenderContext & context);

        void setShaderProgram(ShaderProgram & shaderProgram);

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

        void init();
        void loadUniformData(const RenderContext & context);

    };
}

#endif /* RIGID3D_RENDERABLE_HPP_ */
