#ifndef RIGID3D_SCENE_HPP_
#define RIGID3D_SCENE_HPP_

#include "Rigid3D/Common/Settings.hpp"

#include <initializer_list>
#include <utility>
#include <unordered_map>
#include <vector>

// Forward declaration
namespace Rigid3D {
    class Renderable;
}

namespace Rigid3D {

    struct BatchInfo {
        unsigned int startIndex;
        unsigned int numIndices;

        BatchInfo() : startIndex(0), numIndices(0) { }
    };

    typedef const char * meshName;
    typedef const char * objFile;

    class Scene {
    public:
        Scene(std::initializer_list<std::pair<meshName, objFile> > list);
        ~Scene();

//        Renderable * createRenderable(meshName);

    private:
        vec3 * positions;
        vec3 * normals;
        vec2 * textureCoords;
        int32 numVertices;

        std::unordered_map<meshName, BatchInfo> meshBatchMap;

        std::vector<Renderable *> renderables;
    };

}

#endif /* RIGID3D_SCENE_HPP_ */
