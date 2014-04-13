#include "MeshConsolidator.hpp"

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <cstring>
#include <cstdlib>

namespace Rigid3D {
    
using namespace std;

//----------------------------------------------------------------------------------------
/**
 * Default constructor
 */
MeshConsolidator::MeshConsolidator()
        : totalPositionBytes(0),
          totalNormalBytes(0),
          vertexPositionDataPtr_head(nullptr),
          vertexPositionDataPtr_tail(nullptr),
          normalDataPtr_head(nullptr),
          normalDataPtr_tail(nullptr) { }


//----------------------------------------------------------------------------------------
/**
 * Constructs a \c MeshConsolidator object from an \c unordered_map with keys equal to
 * c-string identifiers, and mapped values equal to Mesh pointers.
 *
 * @param list
 */
MeshConsolidator::MeshConsolidator(initializer_list<pair<const char *, const Mesh *> > list)
        : totalPositionBytes(0), totalNormalBytes(0) {

    unordered_map<const char *, const Mesh *> meshMap;
    for(auto key_value : list) {
        meshMap[key_value.first] = key_value.second;
    }

    processMeshes(meshMap);
}

//----------------------------------------------------------------------------------------
/**
 * Constructs a \c MeshConsolidator object from an \c unordered_map with keys equal to
 * c-string identifiers, and mapped values equal to Wavefront .obj file names.
 *
 * @param list
 */
MeshConsolidator::MeshConsolidator(initializer_list<pair<const char *, const char *> > list)
        : totalPositionBytes(0), totalNormalBytes(0) {

    // Need to keep Mesh objects in memory for processing until the end of this block.
    // Use vector<shared_ptr<Mesh>> as memory requirements could be large for some Meshes.
    // Meshes will auto-destruct at the end of this method when vector goes out of scope.
    vector<shared_ptr<Mesh> > meshVector;
    meshVector.resize(list.size());

    unordered_map<const char *, const Mesh *> meshMap;
    int i = 0;
    for(auto key_value : list) {
        const char * meshId = key_value.first;
        const char * meshFileName = key_value.second;
        meshVector[i] = make_shared<Mesh>(meshFileName);
        meshMap[meshId] = meshVector[i].get();
        i++;
    }

    processMeshes(meshMap);
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::processMeshes(const unordered_map<const char *, const Mesh *> & meshMap) {

    // Calculate the total number of bytes for both vertex and normal data.
    for(auto key_value: meshMap) {
        const Mesh & mesh = *(key_value.second);
        totalPositionBytes += mesh.getNumVertexPositionBytes();
        totalNormalBytes += mesh.getNumVertexNormalBytes();
    }

    // Allocate memory for vertex position data.
    vertexPositionDataPtr_head = shared_ptr<float>((float *)malloc(totalPositionBytes), free);
    if (vertexPositionDataPtr_head.get() == (float *)0) {
        throw Rigid3DException("Unable to allocate system memory within method MeshConsolidator::processMeshes");
    }

    // Allocate memory for normal data.
    normalDataPtr_head = shared_ptr<float>((float *)malloc(totalNormalBytes), free);
    if (vertexPositionDataPtr_head.get() == (float *)0) {
        throw Rigid3DException("Unable to allocate system memory within method MeshConsolidator::processMeshes");
    }

    // Assign pointers to beginning of memory blocks.
    vertexPositionDataPtr_tail = vertexPositionDataPtr_head.get();
    normalDataPtr_tail = normalDataPtr_head.get();

    for(auto key_value : meshMap) {
        const char * meshId = key_value.first;
        const Mesh & mesh = *(key_value.second);
        consolidateMesh(meshId, mesh);
    }
}

//----------------------------------------------------------------------------------------
MeshConsolidator::~MeshConsolidator() {
    // All resources auto freed by shared pointers.
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::consolidateMesh(const char * meshId, const Mesh & mesh) {
    unsigned int startIndex = (unsigned int)((vertexPositionDataPtr_tail - vertexPositionDataPtr_head.get()) / num_floats_per_vertex);
    unsigned int numIndices = mesh.getNumVertexPositions();

    memcpy(vertexPositionDataPtr_tail, mesh.getVertexPositionDataPtr(), mesh.getNumVertexPositionBytes());
    vertexPositionDataPtr_tail += mesh.getNumVertexPositionBytes() / sizeof(float);

    memcpy(normalDataPtr_tail, mesh.getVertexNormalDataPtr(), mesh.getNumVertexNormalBytes());
    normalDataPtr_tail += mesh.getNumVertexNormalBytes() / sizeof(float);

    batchInfoMap[meshId] = BatchInfo(startIndex, numIndices);
}

//----------------------------------------------------------------------------------------
/**
 * Appends to \c batchInfoMap key-value pairs consisting of c-string identifiers as keys,
 * and \c BatchInfo objects as values, where the c-string identifiers were recorded as part of
 * MeshConsolidation construction.
 *
 * @param batchInfoMap
 *
 * @see BatchInfo
 */
void MeshConsolidator::getBatchInfo(unordered_map<const char *, BatchInfo> & batchInfoMap) const {
    for(const auto & key_value : this->batchInfoMap) {
        const char * meshId = key_value.first;
        BatchInfo batchInfo = key_value.second;
        batchInfoMap[meshId] = batchInfo;
    }
}

//----------------------------------------------------------------------------------------
/**
 * @return the starting memory location for all consolidated \c Mesh vertex data.
 */
const float * MeshConsolidator::getVertexPositionDataPtr() const {
    return vertexPositionDataPtr_head.get();
}

//----------------------------------------------------------------------------------------
/**
 * @return the starting memory location for all consolidated \c Mesh normal data.
 */
const float * MeshConsolidator::getVertexNormalDataPtr() const {
    return normalDataPtr_head.get();
}

//----------------------------------------------------------------------------------------
/**
 * @return the total number of bytes of all consolidated \c Mesh vertex data.
 */
unsigned long MeshConsolidator::getNumVertexPositionBytes() const {
    return totalPositionBytes;
}

//----------------------------------------------------------------------------------------
/**
 * @return the total number of bytes of all consolidated \c Mesh normal data.
 */
unsigned long MeshConsolidator::getNumVertexNormalBytes() const {
    return totalNormalBytes;
}

} // end namespace Rigid3D
