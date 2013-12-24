#ifndef RIGID3D_RENDERABLE_HPP_
#define RIGID3D_RENDERABLE_HPP_

// Forward Declarations.
namespace Rigid3D {
    class ModelTransform;
    class Scene;
    class ShaderProgram;
    struct BatchInfo;
    struct MaterialProperties;
}

namespace Rigid3D {

    class Renderable {
    public:


    private:
        friend Scene;

        Renderable(const Scene & scene, const BatchInfo & batchInfo);

        Scene * const scene;
        ShaderProgram * shader;
        MaterialProperties material;
        ModelTransform modelTransform;
        BatchInfo batchInfo;
    };

}



#endif /* RIGID3D_RENDERABLE_HPP_ */
