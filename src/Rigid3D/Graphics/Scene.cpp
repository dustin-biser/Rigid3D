#include "Scene.hpp"

#include <Rigid3D/Graphics/ObjFileLoader.hpp>
#include <Rigid3D/Graphics/Renderable.hpp>

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <sstream>
#include <cstring>

namespace Rigid3D {

using std::stringstream;
using std::endl;
using std::memcpy;

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
    // 1. Construct a local unordered_map<meshName, MeshData> meshMap.
    // 2. For each pair in init_list construct a MeshData struct using ObjFileLoader, and
    //    insert the [meshName, MeshData] pair into meshMap.
    // 3. For each [meshName, Mesh] pair in meshMap:
    //      + Copy vertex data from Mesh to Scene object
    //      + Construct a BatchInfo object
    //      + Insert [meshName, BatchInfo] pair into meshBatchMap.

    unordered_map<string, MeshData> meshDataMap;

    for(const MeshInfo & meshInfo : meshList) {
        MeshData meshData;
        ObjFileLoader::load(meshData, meshInfo.objFile);

        checkMeshNameIsUnique(meshDataMap, meshInfo.meshName);

        meshDataMap[meshInfo.meshName] = meshData;
    }

    computeTotalVerticesAndIndices(meshDataMap);

    copyMeshDataMapData(meshDataMap);

    deleteMeshDataMapData(meshDataMap);
}

//----------------------------------------------------------------------------------------
Scene::~Scene() {
    MeshData * data[] = { &aggregateData, &aggregateTexturedData };

    // Delete all mesh vertex data.
    for (int i = 0; i < 2; i++) {
        delete [] data[i]->positions;
        delete [] data[i]->normals;
        delete [] data[i]->textureCoords;
        delete [] data[i]->indices;
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
void Scene::checkMeshNameExists(const string & meshName) {
    if (meshBatchMap.count(meshName) == 0) {
        stringstream errorMessage;
        errorMessage <<"Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage <<"Mesh with name \'" << meshName <<  "\' does not exist. ";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::checkMeshNameIsUnique(const unordered_map<string, MeshData> & meshDataMap,
        const string & meshName) {

    if (meshDataMap.count(meshName) > 0) {
        stringstream errorMessage;
        errorMessage <<"Exception thrown from class Rigid3D::Scene" << endl;
        errorMessage <<"Duplicate mesh name \'" << meshName <<  "\' not allowed. ";
        errorMessage << "Mesh names must be unique.";

        throw Rigid3DException(errorMessage.str());
    }
}

//----------------------------------------------------------------------------------------
void Scene::computeTotalVerticesAndIndices(const unordered_map<string, MeshData> & meshDataMap) {
    aggregateData.numVertices = 0;
    aggregateTexturedData.numVertices = 0;

    for(auto & key_value : meshDataMap ) {
        const MeshData & meshData = key_value.second;
        if (meshData.hasTextureCoords) {
            aggregateTexturedData.numVertices += meshData.numVertices;
            aggregateTexturedData.numIndices += meshData.numIndices;
        } else {
            aggregateData.numVertices += meshData.numVertices;
            aggregateData.numIndices += meshData.numIndices;
        }
    }

}

//----------------------------------------------------------------------------------------
// Copy all mesh data into data arrays held by this Scene.
void Scene::copyMeshDataMapData(const unordered_map<string, MeshData> & meshDataMap) {
    // Expand vertex data arrays so they can hold all the mesh data.
    MeshData * data[] = { &aggregateData, &aggregateTexturedData };
    for (int i = 0; i < 2; i++) {
        data[i]->positions = new vec3[data[i]->numVertices];
        data[i]->normals = new vec3[data[i]->numVertices];
        data[i]->textureCoords = new vec2[data[i]->numVertices];
        data[i]->indices = new uint32[data[i]->numIndices];
    }

    uint32 nonTexturedDataOffset = 0;
    uint32 texturedDataOffSet = 0;
    uint32 dataOffSet[2] = { nonTexturedDataOffset, texturedDataOffSet };
    uint32 indexOffSet[2] = { nonTexturedDataOffset, texturedDataOffSet };
    int i = 0;

    // Copy the mesh data.
    // Keep track of BatchInfo for each Renderable.
    for(auto & key_value : meshDataMap) {
        const MeshData & meshData = key_value.second;

        i = (meshData.hasTextureCoords) ? 1 : 0;

        memcpy(data[i]->positions + dataOffSet[i],
                meshData.positions, meshData.numVertices);

        memcpy(data[i]->normals + dataOffSet[i],
                meshData.normals , meshData.numVertices);


        if (meshData.hasTextureCoords) {
            memcpy(data[i]->textureCoords + dataOffSet[i],
                    meshData.textureCoords , meshData.numVertices);
        }

        memcpy(data[i]->indices + indexOffSet[i],
                meshData.indices, meshData.numIndices);

        // Map mesh name to a BatchInfo for easy look up later.
        BatchInfo batchInfo;
        batchInfo.startIndex = indexOffSet[i];
        batchInfo.numIndices = meshData.numIndices;
        string meshName = key_value.first;
        meshBatchMap[meshName] = batchInfo;

        // Increment offsets.
        dataOffSet[i] += meshData.numVertices;
        indexOffSet[i] += meshData.numIndices;
    }

}

//----------------------------------------------------------------------------------------
void Scene::deleteMeshDataMapData(unordered_map<string, MeshData> & meshDataMap) {
    for(auto & key_value : meshDataMap) {
        MeshData & meshData = key_value.second;
        delete [] meshData.positions;
        delete [] meshData.normals;
        delete [] meshData.textureCoords;
        delete [] meshData.indices;
        meshData.numVertices = 0;
        meshData.numIndices = 0;
        meshData.hasTextureCoords = false;
    }
}

} // end namespace Rigid3D.
