#include "Scene.hpp"

#include <Rigid3D/Graphics/ObjFileLoader.hpp>
#include <Rigid3D/Graphics/Renderable.hpp>
#include <Rigid3D/Graphics/ShaderProgram.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>
#include <Rigid3D/Graphics/Light.hpp>

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <cstddef>
#include <sstream>
#include <cstring>


namespace Rigid3D {

using std::stringstream;
using std::endl;
using std::memcpy;

//----------------------------------------------------------------------------------------
const uint Scene::MAX_NUM_LIGHTS = 10;

//-- Vertex Attribute Layout Locations.
const GLuint Scene::POSITION_ATTRIBUTE_INDEX = 0;
const GLuint Scene::NORMAL_ATTRIBUTE_INDEX = 1;
const GLuint Scene::TEXTURE_COORD_ATTRIBUTE_INDEX = 2;


//----------------------------------------------------------------------------------------
MeshInfo::MeshInfo(const string & meshName, const string & objFile)
    : meshName(meshName),
      objFile(objFile) {

}

//----------------------------------------------------------------------------------------
Scene::Scene()
    : vao_nonTextured(0),
      vbo_nonTextured(0),
      indexBuffer_nonTextured(0),
      vao_textured(0),
      vbo_textured(0),
      indexBuffer_textured(0) {

}

//----------------------------------------------------------------------------------------
/**
 * Constructor
 * Constructs a Scene using  an 'initiaizizer_list', where each element of the list
 * is a MeshInfo object containing a 'meshName' 'string' and an 'objFile' 'string'.
 * This will form the set of all meshes that can be rendered as part of the Scene.
 *
 * Example:
 * \code
 * Scene scene = { MeshInfo("mesh1", "path/to/mesh/data1.obj"),
 *                 MeshInfo("mesh2", "path/to/mesh/data2.obj")
 *                 ...
 *                 MeshInfo("meshN", "path/to/mesh/dataN.obj")
 *               };
 * \endcode
 *
 * The 'meshName' 'string' identifier for each mesh will be stored internally, and
 * can be used to generate Renderable instances of the mesh using
 * Scene::createRenderable(const string & meshName).
 *
 * @see MeshInfo
 *
 * @param meshList
 */
Scene::Scene(initializer_list<MeshInfo> meshList) {

    for(const MeshInfo & meshInfo : meshList) {
        MeshData meshData;
        ObjFileLoader::load(meshData, meshInfo.objFile);

        checkMeshNameIsUnique(meshDataMap, meshInfo.meshName);
        meshDataMap[meshInfo.meshName] = meshData;

        createBatchInfo(meshInfo.meshName, meshData);

        if (meshData.hasTextureCoords) {
            processTexturedMeshData(meshInfo.meshName, meshData);
        } else {
            processNonTexturedMeshData(meshInfo.meshName, meshData);
        }
    }

    createVertexArrayObjects();

    enableVertexAttributeArrays();

    createVertexBuffersAndCopyData();

    createIndexBuffersAndCopyData();

    deleteVertexAndIndexData();
}

//----------------------------------------------------------------------------------------
Scene::~Scene() {
    //-- Delete all MeshData.
    for(auto & key_value : meshDataMap) {
        MeshData meshData = key_value.second;

        delete [] meshData.positionSet;
        meshData.positionSet = nullptr;

        delete [] meshData.normalSet;
        meshData.normalSet = nullptr;

        delete [] meshData.textureCoordSet;
        meshData.textureCoordSet = nullptr;

        delete [] meshData.positionIndices;
        meshData.positionIndices = nullptr;

        delete [] meshData.normalIndices;
        meshData.normalIndices = nullptr;

        delete [] meshData.textureCoordIndices;
        meshData.textureCoordIndices = nullptr;

        meshData.numFaces = 0;
        meshData.numPositions = 0;
        meshData.numNormals = 0;
        meshData.numTextureCoords = 0;
        meshData.hasTextureCoords = false;
    }

    //-- Delete all Renderables.
    for(Renderable * renderable : renderables_textured) {
        delete renderable;
    }

    for(Renderable * renderable : renderables_nonTextured) {
        delete renderable;
    }

    //-- Delete all Lights.
    for(Light * light : lights) {
        delete light;
    }
}

//----------------------------------------------------------------------------------------
/**
 * Creates a Renderable object with properties tailored from a RenderableSpec.
 *
 * @warning Users should never call delete on the pointer returned from this
 * method.  Memory associated with a Renderable will automatically be freed once
 * the Scene's destructor is called.
 *
 * @param spec - specification properties for the Renderable to be created.
 * @return a pointer to a new Renderable object.
 */
Renderable * Scene::createRenderable(const RenderableSpec & spec) {
    checkMeshNameExists(spec.meshName);
    checkRenderableSpecIsValid(spec);

    Renderable * renderable = new Renderable(*this, spec);

    if(meshDataMap[spec.meshName].hasTextureCoords) {
        renderables_textured.push_back(renderable);
    } else {
        renderables_nonTextured.push_back(renderable);
    }

    return renderable;
}

//----------------------------------------------------------------------------------------
void Scene::checkMeshNameExists(const string & meshName) const {
    if (meshBatchMap.count(meshName) == 0) {
        stringstream errorMessage;
        errorMessage << "Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage << "Mesh with name \'" << meshName <<  "\' does not exist. ";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
        const string & meshName) const {

    if (meshDataMap.count(meshName) > 0) {
        stringstream errorMessage;
        errorMessage << "Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage << "Duplicate mesh name \'" << meshName <<  "\' not allowed. ";
        errorMessage << "Mesh names must be unique.";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::checkRenderableSpecIsValid(const RenderableSpec & spec) const {

    if ( (spec.shader == 0) || (spec.shader == nullptr) ) {
        stringstream errorMessage;
        errorMessage << "Exception thrown from class Rigid3D::Scene." << endl;
        errorMessage << "RenderableSpec::shader cannot be null.";

        throw Rigid3DException(errorMessage.str());
    }

}

//----------------------------------------------------------------------------------------
void Scene::createBatchInfo(const string & meshName, const MeshData & meshData) {
    BatchInfo batchInfo;
    uint16 numIndicesPerFace = 3;

    batchInfo.numIndices = meshData.numFaces * numIndicesPerFace;

    if (meshData.hasTextureCoords) {
        // The starting index offset for this mesh begins at the current end of the
        // indexVector_textured.
        batchInfo.startIndex = indexVector_textured.size();
        batchInfo.baseVertex = vertexVector_textured.size();

    } else {
        // The starting index offset for this mesh begins at the current end of the
        // indexVector.
        batchInfo.startIndex = indexVector_nonTextured.size();
        batchInfo.baseVertex = vertexVector_nonTextured.size();
    }

    meshBatchMap[meshName] = batchInfo;
}

//----------------------------------------------------------------------------------------
// Processes the vertex data and indices for a non-textured mesh.
void Scene::processNonTexturedMeshData(const string & meshName, const MeshData & meshData) {
    unordered_map<uvec2, uint16, Hasher> indexMap;
    uvec2 indexPair; // (positionIndex, normalIndex).
    Vertex vertex;
    uint16 index = 0;
    uint8 numIndicesPerFace = 3;

    uint16 posIndex;
    uint16 normalIndex;

    for (uint32 face_i = 0; face_i < meshData.numFaces; face_i++) {
        for (uint8 j = 0; j < numIndicesPerFace; j++) {
            posIndex = meshData.positionIndices[face_i][j];
            normalIndex = meshData.normalIndices[face_i][j];
            indexPair = uvec2(posIndex, normalIndex);

            if (indexMap.count(indexPair) == 0) {
                // First time seeing this indexPair for the given mesh.
                vertex.position = meshData.positionSet[indexPair[0]];
                vertex.normal = meshData.normalSet[indexPair[1]];
                vertexVector_nonTextured.push_back(vertex);

                indexMap[indexPair] = index;
                indexVector_nonTextured.push_back(index);
                index++;

            } else {
                // Get the previously stored index for the indexPair.
                indexVector_nonTextured.push_back(indexMap[indexPair]);
            }

        }
    }
}

//----------------------------------------------------------------------------------------
// Processes the vertex data and indices for a textured mesh.
void Scene::processTexturedMeshData(const string & meshName, const MeshData & meshData) {
    unordered_map<uvec3, uint16, Hasher> indexMap;
    uvec3 indexTriplet; // (positionIndex, normalIndex, textureCoordIndex).
    TexturedVertex vertex;
    uint16 index = 0;
    uint8 numIndicesPerFace = 3;

    uint16 posIndex;
    uint16 normalIndex;
    uint16 textureCoordIndex;

    for (uint32 face_i = 0; face_i < meshData.numFaces; face_i++) {
        for (uint8 j = 0; j < numIndicesPerFace; j++) {
            posIndex = meshData.positionIndices[face_i][j];
            normalIndex = meshData.normalIndices[face_i][j];
            textureCoordIndex = meshData.textureCoordIndices[face_i][j];
            indexTriplet = uvec3(posIndex, normalIndex, textureCoordIndex);

            if (indexMap.count(indexTriplet) == 0) {
                // First time seeing this indexTriplet for the given mesh.
                vertex.position = meshData.positionSet[indexTriplet[0]];
                vertex.normal = meshData.normalSet[indexTriplet[1]];
                vertex.textureCoord = meshData.textureCoordSet[indexTriplet[2]];
                vertexVector_textured.push_back(vertex);

                indexMap[indexTriplet] = index;
                indexVector_textured.push_back(index);
                index++;

            } else {
                // Get the previously stored index for the indexTriplet.
                indexVector_textured.push_back(indexMap[indexTriplet]);
            }

        }
    }
}

//----------------------------------------------------------------------------------------
void Scene::createVertexArrayObjects() {
    glGenVertexArrays(1, &vao_nonTextured);
    glGenVertexArrays(1, &vao_textured);
}

//----------------------------------------------------------------------------------------
void Scene::enableVertexAttributeArrays() {
    glBindVertexArray(vao_nonTextured);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);

    glBindVertexArray(vao_textured);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIBUTE_INDEX);

    glBindVertexArray(0);

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
void Scene::createVertexBuffersAndCopyData() {
    GLsizeiptr numBytes;

    // Copy interleaved non-textured vertex data into OpenGL buffer.
    glGenBuffers(1, &vbo_nonTextured);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_nonTextured);
    numBytes = vertexVector_nonTextured.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, numBytes, vertexVector_nonTextured.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao_nonTextured);
    glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void *>(offsetof(struct Vertex, position)));
    glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void *>(offsetof(struct Vertex, normal)));

    // Copy interleaved textured vertex data into OpenGL buffer.
    glGenBuffers(1, &vbo_textured);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textured);
    numBytes = vertexVector_textured.size() * sizeof(TexturedVertex);
    glBufferData(GL_ARRAY_BUFFER, numBytes, vertexVector_textured.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao_textured);
    glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, position)));
    glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, normal)));
    glVertexAttribPointer(TEXTURE_COORD_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, textureCoord)));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
void Scene::createIndexBuffersAndCopyData() {
    // Generate the index buffer names.
    glGenBuffers(1, &indexBuffer_nonTextured);
    glGenBuffers(1, &indexBuffer_textured);

    GLsizeiptr numBytes;

    // Copy indices into index buffer for non-textured meshes.
    glBindVertexArray(vao_nonTextured);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_nonTextured);
    numBytes = indexVector_nonTextured.size() * sizeof(uint16);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, indexVector_nonTextured.data(), GL_STATIC_DRAW);

    // Copy indices into index buffer for textured meshes.
    glBindVertexArray(vao_textured);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_textured);
    numBytes = indexVector_textured.size() * sizeof(uint16);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, indexVector_textured.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
void Scene::deleteVertexAndIndexData() {
    vertexVector_nonTextured.clear();
    vertexVector_textured.clear();
    indexVector_nonTextured.clear();
    indexVector_textured.clear();
}

//----------------------------------------------------------------------------------------
void Scene::render(const Camera & camera) {
    // Record the previously bound vao.
    GLint prev_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vao);

    // Render textured Renderables.
    glBindVertexArray(vao_textured);
    for(Renderable * r : renderables_textured) {
        render(*r, camera);
    }

    // Render non-textured Renderables.
    glBindVertexArray(vao_nonTextured);
    for(Renderable * r : renderables_nonTextured) {
        render(*r, camera);
    }

    // Restore the previously bound vao and indexBuffer.
    glBindVertexArray(prev_vao);

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
void Scene::render(Renderable & r, const Camera & camera) {
    if (r.cull) return;

    BatchInfo batchInfo;

    batchInfo = meshBatchMap[r.meshName];

    r.loadShaderUniforms(camera);

    r.shader->enable();
    glDrawElementsBaseVertex(GL_TRIANGLES, batchInfo.numIndices, GL_UNSIGNED_SHORT,
            reinterpret_cast<void *>(batchInfo.startIndex * sizeof(uint16)),
            batchInfo.baseVertex);
    r.shader->disable();

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
Light * Scene::createLight(const LightSpec & spec) {
    Light * light = new Light(*this, spec);
    lights.push_back(light);

    return light;
}

} // end namespace Rigid3D.
