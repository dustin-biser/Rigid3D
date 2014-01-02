#include "Scene.hpp"

#include <Rigid3D/Graphics/ObjFileLoader.hpp>
#include <Rigid3D/Graphics/Renderable.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <cstddef>
#include <sstream>
#include <cstring>

namespace Rigid3D {

using std::stringstream;
using std::endl;
using std::memcpy;

// Static class variable definitions.
const GLuint Scene::positionVertexAttributeIndex = 0;
const GLuint Scene::normalVertexAttributeIndex = 1;
const GLuint Scene::textureCoordVertexAttributeIndex = 2;

//----------------------------------------------------------------------------------------
MeshInfo::MeshInfo(const string & meshName, const string & objFile)
    : meshName(meshName),
      objFile(objFile) {

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
    // Delete all MeshData.
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

    // Delete all Renderables.
    for(Renderable * renderable : renderables) {
        delete renderable;
    }
}

//----------------------------------------------------------------------------------------
/**
 * Creates a Renderable object representing an instance of the Mesh with name 'meshName'.
 *
 * @warning Users should never call delete on the pointer returned from this
 * method.  Memory associated with a Renderable will automatically be freed once
 * the Scene destructor is called.
 *
 * @param meshName
 * @return a pointer to a new Renderable.
 */
Renderable * Scene::createRenderable(const string & meshName) {
    checkMeshNameExists(meshName);

    BatchInfo batchInfo = meshBatchMap[meshName];

    Renderable * renderable = new Renderable(*this, meshName, batchInfo);
    renderables.push_back(renderable);

    return renderable;
}

//----------------------------------------------------------------------------------------
void Scene::checkMeshNameExists(const string & meshName) const {
    if (meshBatchMap.count(meshName) == 0) {
        stringstream errorMessage;
        errorMessage <<"Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage <<"Mesh with name \'" << meshName <<  "\' does not exist. ";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
        const string & meshName) const {

    if (meshDataMap.count(meshName) > 0) {
        stringstream errorMessage;
        errorMessage <<"Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage <<"Duplicate mesh name \'" << meshName <<  "\' not allowed. ";
        errorMessage << "Mesh names must be unique.";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::createBatchInfo(const string & meshName, const MeshData & meshData) {
    BatchInfo batchInfo;
    int32 numIndicesPerFace = 3;

    batchInfo.numIndices = meshData.numFaces * numIndicesPerFace;

    if (meshData.hasTextureCoords) {
        // The starting index for this mesh begins at the current end of the
        // texturedIndices vector.
        batchInfo.startIndex = texturedIndexVector.size();

    } else {
        // The starting index for this mesh begins at the current end of the
        // indexVector.
        batchInfo.startIndex = indexVector.size();
    }

    meshBatchMap[meshName] = batchInfo;
}

//----------------------------------------------------------------------------------------
// Processes the vertex data and indices for a non-textured mesh.
// Appends the mesh's vertex set to the vertexVector vector.  Determines the
// indices of vertexVector's elements to use for rendering the mesh, and appends
// these indices to indexVector.  Also constructs a BatchInfo and stores
// it in meshBatchMap.
void Scene::processNonTexturedMeshData(const string & meshName, const MeshData & meshData) {
    unordered_map<ivec2, uint32, Hasher> indexMap;
    ivec2 indexPair; // (positionIndex, normalIndex).
    Vertex vertex;
    uint32 vertexIndex;
    int32 numIndicesPerFace = 3;

    int32 posIndex;
    int32 normalIndex;

    for (int32 face_i = 0; face_i < meshData.numFaces; face_i++) {
        for (int32 j = 0; j < numIndicesPerFace; j++) {
            posIndex = meshData.positionIndices[face_i][j];
            normalIndex = meshData.normalIndices[face_i][j];
            indexPair = ivec2(posIndex, normalIndex);

            if (indexMap.count(indexPair) == 0) {
                // First time seeing this indexPair for the given mesh.

                vertexIndex = vertexVector.size();
                vertex.position = meshData.positionSet[indexPair[0]];
                vertex.normal = meshData.normalSet[indexPair[1]];

                vertexVector.push_back(vertex);

                indexMap[indexPair] = vertexIndex;

            } else {
                // Get the previously stored vertexIndex for the indexPair.
                vertexIndex = indexMap[indexPair];
            }

            indexVector.push_back(vertexIndex);
        }
    }
}

//----------------------------------------------------------------------------------------
// Processes the vertex data and indices for a textured mesh.
// Appends the mesh's vertex set to the texturedVertexVector vector.  Determines the
// indices of texturedVertexVector's elements to use for rendering the mesh, and appends
// these indices to texturedIndexVector.  Also constructs a BatchInfo and stores
// it in meshBatchMap.
void Scene::processTexturedMeshData(const string & meshName, const MeshData & meshData) {
    unordered_map<ivec3, uint32, Hasher> indexMap;
    ivec3 indexTriplet; // (positionIndex, normalIndex, textureCoordIndex).
    TexturedVertex vertex;
    uint32 vertexIndex;
    int32 numIndicesPerFace = 3;

    int32 posIndex;
    int32 normalIndex;
    int32 textureCoordIndex;

    for (int32 face_i = 0; face_i < meshData.numFaces; face_i++) {
        for (int32 j = 0; j < numIndicesPerFace; j++) {
            posIndex = meshData.positionIndices[face_i][j];
            normalIndex = meshData.normalIndices[face_i][j];
            textureCoordIndex = meshData.textureCoordIndices[face_i][j];
            indexTriplet = ivec3(posIndex, normalIndex, textureCoordIndex);

            if (indexMap.count(indexTriplet) == 0) {
                // First time seeing this indexTriplet for the given mesh.

                vertexIndex = texturedVertexVector.size();
                vertex.position = meshData.positionSet[indexTriplet[0]];
                vertex.normal = meshData.normalSet[indexTriplet[1]];
                vertex.textureCoord = meshData.textureCoordSet[indexTriplet[2]];

                texturedVertexVector.push_back(vertex);

                indexMap[indexTriplet] = vertexIndex;

            } else {
                // Get the previously stored vertexIndex for the indexTriplet.
                vertexIndex = indexMap[indexTriplet];
            }

            texturedIndexVector.push_back(vertexIndex);
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
    glEnableVertexAttribArray(positionVertexAttributeIndex);
    glEnableVertexAttribArray(normalVertexAttributeIndex);

    glBindVertexArray(vao_textured);
    glEnableVertexAttribArray(positionVertexAttributeIndex);
    glEnableVertexAttribArray(normalVertexAttributeIndex);
    glEnableVertexAttribArray(textureCoordVertexAttributeIndex);

    glBindVertexArray(0);

    checkGLErrors(__FILE__, __LINE__);
}

//----------------------------------------------------------------------------------------
void Scene::createVertexBuffersAndCopyData() {
    GLsizeiptr numBytes;

    // Copy interleaved non-textured vertex data into OpenGL buffer.
    glGenBuffers(1, &vbo_nonTextured);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_nonTextured);
    numBytes = vertexVector.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, numBytes, vertexVector.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao_nonTextured);
    glVertexAttribPointer(positionVertexAttributeIndex, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void *>(offsetof(struct Vertex, position)));
    glVertexAttribPointer(normalVertexAttributeIndex, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void *>(offsetof(struct Vertex, normal)));

    // Copy interleaved textured vertex data into OpenGL buffer.
    glGenBuffers(1, &vbo_textured);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textured);
    numBytes = texturedVertexVector.size() * sizeof(TexturedVertex);
    glBufferData(GL_ARRAY_BUFFER, numBytes, texturedVertexVector.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao_textured);
    glVertexAttribPointer(positionVertexAttributeIndex, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, position)));
    glVertexAttribPointer(normalVertexAttributeIndex, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, normal)));
    glVertexAttribPointer(textureCoordVertexAttributeIndex, 2, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex),
            reinterpret_cast<void *>(offsetof(struct TexturedVertex, textureCoord)));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    checkGLErrors(__FILE__, __LINE__);
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
    numBytes = indexVector.size() * sizeof(uint32);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, indexVector.data(), GL_STATIC_DRAW);

    // Copy indices into index buffer for textured meshes.
    glBindVertexArray(vao_textured);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_textured);
    numBytes = texturedIndexVector.size() * sizeof(uint32);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, texturedIndexVector.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    checkGLErrors(__FILE__, __LINE__);
}

//----------------------------------------------------------------------------------------
void Scene::deleteVertexAndIndexData() {
    vertexVector.clear();
    texturedVertexVector.clear();
    indexVector.clear();
    texturedIndexVector.clear();
}

} // end namespace Rigid3D.
