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

using std::initializer_list;
using std::pair;
using std::unordered_map;
using std::vector;

    struct BatchInfo {
        unsigned int startIndex;
        unsigned int numIndices;

        BatchInfo() : startIndex(0), numIndices(0) { }
    };

    class Scene {
    typedef const char * meshName;
    typedef const char * objFile;

    public:
        Scene(initializer_list<pair<meshName, objFile> > list);
        ~Scene();

//        Renderable * createRenderable(meshName);

    private:
        vec3 * positions;
        vec3 * normals;
        vec2 * textureCoords;
        int32 numVertices;

        unordered_map<meshName, BatchInfo> meshBatchMap;

        vector<Renderable *> renderables;
    };

}

#endif /* RIGID3D_SCENE_HPP_ */
