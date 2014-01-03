#ifndef RIGID3D_SCENE_HPP_
#define RIGID3D_SCENE_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/ObjFileLoader.hpp>

#include <GL/glew.h>

#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <string>

// Forward declarations.
namespace Rigid3D {
    class Renderable;
    class Camera;
    struct RenderableSpec;
}

namespace Rigid3D {

using std::initializer_list;
using std::unordered_map;
using std::string;
using std::vector;

    struct BatchInfo {
        uint16 startIndex;
        uint16 numIndices;
        uint16 baseVertex;
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
        Scene();
        Scene(initializer_list<MeshInfo> meshList);
        ~Scene();

        Renderable * createRenderable(const RenderableSpec & spec);

        void render(const Camera & camera);

    private:
        friend class Renderable;

        // Layout location for vertex shader position attribute array.
        static const GLuint positionAttributeLocation;

        // Layout location for vertex shader normal attribute array.
        static const GLuint normalAttributeLocation;

        // Layout location for vertex shader texture coordinate attribute array.
        static const GLuint textureCoordAttributeLocation;

        // Layout location for vertex shader Model-View-Matrix uniform.
        static const GLuint modelViewMatrixUniformLocation;

        // Layout location for vertex shader Normal-Matrix uniform.
        static const GLuint normalMatrixUniformLocation;

        // Layout location for vertex shader Projection-Matrix uniform.
        static const GLuint projectionMatrixUniformLocation;

        vector<Vertex> vertexVector_nonTextured;
        vector<TexturedVertex> vertexVector_textured;

        vector<uint16> indexVector_nonTextured;
        vector<uint16> indexVector_textured;

        unordered_map<string, BatchInfo> meshBatchMap;
        unordered_map<string, MeshData> meshDataMap;

        vector<Renderable *> renderables_nonTextured;
        vector<Renderable *> renderables_textured;

        GLuint vao_nonTextured;
        GLuint vbo_nonTextured;
        GLuint indexBuffer_nonTextured;

        GLuint vao_textured;
        GLuint vbo_textured;
        GLuint indexBuffer_textured;


        void checkMeshNameExists(const string & meshName) const;

        void checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
                const string & meshName) const;

        void checkRenderableSpecIsValid(const RenderableSpec & spec) const;

        void processNonTexturedMeshData(const string & meshName, const MeshData & meshData);

        void processTexturedMeshData(const string & meshName, const MeshData & meshData);

        void createBatchInfo(const string & meshName, const MeshData & meshData);

        void createVertexArrayObjects();

        void enableVertexAttributeArrays();

        void createVertexBuffersAndCopyData();

        void createIndexBuffersAndCopyData();

        void deleteVertexAndIndexData();

        void render(Renderable & r, const Camera & camera);
    };

}

#endif /* RIGID3D_SCENE_HPP_ */
