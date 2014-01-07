#ifndef RIGID3D_RENDERABLE_HPP_
#define RIGID3D_RENDERABLE_HPP_

#include <Rigid3D/Math/Transform.hpp>
#include <Rigid3D/Graphics/Material.hpp>

#include <string>

// Forward Declarations
namespace Rigid3D {
    class Scene;
    class ShaderProgram;
    class Camera;
}

namespace Rigid3D {
    using std::string;

    /**
     * Specification details to be used for constructing a Renderable.
     *
     * @note 'cull' defaults to false.
     */
    struct RenderableSpec {
        /// Mesh name that identifies the mesh to use for rendering the  Renderable.
        string meshName;

        /// Shader to use when rendering the Renderable.
        ShaderProgram * shader;

        /// Material lighting properties to use when rendering the Renderable.
        Material material;

        /// Transform for positioning, orienting, and scaling the Renderable.
        Transform transform;

        /**
         *  If true, the corresponding Renderable's vertex data will not be sent
         *  through the rendering pipeline upon calling
         *  Rigid3D::Renderable::render or Rigid3D::Scene::render.
         *
         *  Default value is false.
         */
        bool cull;

        RenderableSpec();
    };

    // Augmented Transform class used for computing a model matrix from a
    // Transform.
    class ModelTransform {
    private:
        friend class Renderable;

        Transform transform;
        mutable mat4 modelMatrix;
        mutable bool recalcModelMatrix;

        ModelTransform();

        void set(const Transform & transform);

        const Transform & getTransform() const;

        mat4 getModelMatrix() const;
    };

    /**
     * An entity that can be rendered to the screen.  Renderables are created
     * using Rigid3D::Scene::createRenderable.
     */
    class Renderable {
    public:
        void setTransform(const Transform & transform);

        void setShader(const ShaderProgram & shader);

        ShaderProgram & getShader() const;

        const Transform & getTransform() const;

    private:
        friend class Scene;

        Scene & scene;
        ShaderProgram * shader;
        string meshName;
        Material material;
        ModelTransform modelTransform;
        bool cull;

        Renderable(Scene & scene, const RenderableSpec & spec);

        void loadShaderUniforms(const Camera & camera);

        void checkShaderIsNotNull(const ShaderProgram * shader) const;
    };

}

#endif /* RIGID3D_RENDERABLE_HPP_ */
