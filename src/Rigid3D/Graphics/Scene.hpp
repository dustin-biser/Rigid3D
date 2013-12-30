#ifndef RIGID3D_SCENE_HPP_
#define RIGID3D_SCENE_HPP_

#include "Rigid3D/Common/Settings.hpp"
#include "Rigid3D/Graphics/ObjFileLoader.hpp"

#include <GL/glew.h>

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
using std::string;

    struct BatchInfo {
        uint32 startIndex;
        uint32 numIndices;
    };

    /**
     * Structure for holding a mesh name and its corresponding Wavefront .obj
     * file.
     */
    struct MeshInfo {
        string meshName; /// Unique mesh identifier
        string objFile;  /// Path to .obj file containing mesh data.

        MeshInfo(const string & meshName, const string & objFile);
    };

    struct Vertex {
       vec3 position;
       vec3 normal;
    };

    struct TexturedVertex {
       vec3 position;
       vec3 normal;
       vec2 textureCoord;
    };

    /**
     * Class for creating and managing all objects that would be part of a
     * rendered scene, including Renderable, Camera, and Light objects.
     */
    class Scene {
    public:
        Scene(initializer_list<MeshInfo> meshList);
        ~Scene();

        Renderable * createRenderable(const string & meshName);

    private:
        MeshData aggregateData;  // Combined vertex data for meshes with no textureCoordinates.
        MeshData aggregateTexturedData;  // Combined vertex data for meshes with textureCoordinates.

        unordered_map<string, BatchInfo> meshBatchMap;
        vector<Renderable *> renderables;

        GLuint vao_nonTextured;
        GLuint vbo_nonTextured;

        GLuint vao_textured;
        GLuint vbo_textured;

        void checkMeshNameExists(const string & meshName);

        void checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
                const string & meshName);

        void copyMeshDataMapData(const unordered_map<string, MeshData> & meshDataMap);

        void computeTotalVerticesAndIndices(const unordered_map<string, MeshData> & meshDataMap);

        void deleteMeshDataMapData(unordered_map<string, MeshData> & meshDataMap);

    };

}

#endif /* RIGID3D_SCENE_HPP_ */
