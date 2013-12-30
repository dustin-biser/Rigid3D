#ifndef RIGID3D_RENDERABLE_HPP_
#define RIGID3D_RENDERABLE_HPP_

#include <Rigid3D/Graphics/ModelTransform.hpp>

#include <string>

// Forward Declarations
namespace Rigid3D {
    class Scene;
    class ShaderProgram;
    struct BatchInfo;
    struct Material;
}

namespace Rigid3D {
    using std::string;

    class Renderable {
    public:


    private:
        friend Scene;

        Renderable(const Scene & scene, const string & meshName, const BatchInfo & batchInfo);

        Scene * const scene;
        string meshName;
        BatchInfo batchInfo;

        ShaderProgram * shader;
        Material material;
        ModelTransform modelTransform;
    };

}



#endif /* RIGID3D_RENDERABLE_HPP_ */
