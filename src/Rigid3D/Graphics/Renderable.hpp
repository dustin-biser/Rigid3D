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
        string meshName;
        ShaderProgram * shader;
        Material material;
        Transform transform;

        /**
         *  If true, the corresponding Renderable's vertex data will not be sent
         *  through the rendering pipeline upon calling
         *  Rigid3D::Renderable::render or Rigid3D::Scene::render.
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

        const Transform & getTransform() const;

    private:
        friend class Scene;

        Scene & scene;
        string meshName;

        ShaderProgram * shader;
        Material material;
        ModelTransform modelTransform;
        bool cull;

        Renderable(const Scene & scene, const RenderableSpec & spec);

        void loadShaderUniforms(const Camera & camera);

    };

}

#endif /* RIGID3D_RENDERABLE_HPP_ */
