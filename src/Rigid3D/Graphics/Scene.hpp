#ifndef RIGID3D_SCENE_HPP_
#define RIGID3D_SCENE_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/ObjFileLoader.hpp>

#include <GL/glew.h>

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Forward declarations.
namespace Rigid3D {
    class Camera;
    class Light;
    class ShaderProgram;
    class Renderable;
    struct LightSpec;
    struct RenderableSpec;
}

namespace Rigid3D {

using std::initializer_list;
using std::string;
using std::unordered_map;
using std::unordered_set;
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

        Light * createLight(const LightSpec & spec);

        void render(const Camera & camera);

    private:
        friend class Renderable;

        // Layout location for vertex shader position attribute array.
        static const GLuint POSITION_ATTRIBUTE_INDEX;

        // Layout location for vertex shader normal attribute array.
        static const GLuint NORMAL_ATTRIBUTE_INDEX;

        // Layout location for vertex shader texture coordinate attribute array.
        static const GLuint TEXTURE_COORD_ATTRIBUTE_INDEX;

        // Maximum number of lights that can contribute to a Scene's lighting.
        static const uint MAX_NUM_LIGHTS;

        vector<Vertex> vertexVector_nonTextured;
        vector<TexturedVertex> vertexVector_textured;

        vector<uint16> indexVector_nonTextured;
        vector<uint16> indexVector_textured;

        unordered_map<string, BatchInfo> meshBatchMap;
        unordered_map<string, MeshData> meshDataMap;

        // Keys are shader program objects (GLuints)
        unordered_map<ShaderProgram *, unordered_set<Renderable *> > shaderRenderableMap;

        vector<Renderable *> renderables_nonTextured;
        vector<Renderable *> renderables_textured;
        vector<Light *> lights;

        GLuint vao_nonTextured;
        GLuint vbo_nonTextured;
        GLuint indexBuffer_nonTextured;

        GLuint vao_textured;
        GLuint vbo_textured;
        GLuint indexBuffer_textured;


        void checkMeshNameExists(const string & meshName) const;

        void checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
                const string & meshName) const;

        void checkShaderIsNotNull(const ShaderProgram * shader) const;

        void processNonTexturedMeshData(const string & meshName, const MeshData & meshData);

        void processTexturedMeshData(const string & meshName, const MeshData & meshData);

        void createBatchInfo(const string & meshName, const MeshData & meshData);

        void createVertexArrayObjects();

        void enableVertexAttributeArrays();

        void createVertexBuffersAndCopyData();

        void createIndexBuffersAndCopyData();

        void deleteVertexAndIndexData();

        void render(Renderable & r, const Camera & camera);

        void loadLightUniformData(ShaderProgram * shader, const Camera & camera);
    };

}

#endif /* RIGID3D_SCENE_HPP_ */
