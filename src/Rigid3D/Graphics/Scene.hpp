#ifndef RIGID3D_SCENE_HPP_
#define RIGID3D_SCENE_HPP_

#include "Rigid3D/Common/Settings.hpp"

#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <string>

// Forward declaration
namespace Rigid3D {
    class Renderable;
}

namespace Rigid3D {

using std::initializer_list;
using std::unordered_map;
using std::vector;
using std::string;

    struct BatchInfo {
        unsigned int startIndex;
        unsigned int numIndices;
    };

    struct MeshInfo {
        string meshName; // Unique mesh identifier
        string objFile;  // Path to .obj file containing mesh vertex data.
    };

    class Scene {
    public:
        Scene(initializer_list<MeshInfo> meshList);
        ~Scene();

//        Renderable * createRenderable(meshName);

    private:
        vec3 * positions;
        vec3 * normals;
        vec2 * textureCoords;
        int32 numVertices;

        unordered_map<string, BatchInfo> meshBatchMap;

        vector<Renderable *> renderables;
    };

}

#endif /* RIGID3D_SCENE_HPP_ */
