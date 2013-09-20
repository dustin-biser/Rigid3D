#include <MeshConsolidator.hpp>
#include <cstring>

using namespace GlUtils;

//----------------------------------------------------------------------------------------
/**
 *
 * @param meshList
 */
MeshConsolidator::MeshConsolidator(initializer_list<Mesh> meshList)
        : totalVertexBytes(0), totalNormalBytes(0) {

    batchInfoVec.resize(meshList.size());

    for(const Mesh &m : meshList) {
        totalVertexBytes += m.getNumVertexBytes();
        totalNormalBytes += m.getNumNormalBytes();
    }

    vertexDataPtr_head = (float *)malloc(totalVertexBytes);
    normalDataPtr_head = (float *)malloc(totalNormalBytes);

    // Assign pointers to beginning of memory blocks.
    vertexDataPtr_tail = vertexDataPtr_head;
    normalDataPtr_tail = normalDataPtr_head;

    unsigned int meshNumber = 0;
    for(const Mesh &m : meshList) {
        consolidateMesh(m, meshNumber);
        meshNumber++;
    }
}

//----------------------------------------------------------------------------------------
MeshConsolidator::~MeshConsolidator() {
    delete vertexDataPtr_head;
    delete normalDataPtr_head;
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::consolidateMesh(const Mesh & mesh, unsigned int meshNumber) {
    unsigned int startIndex = (vertexDataPtr_tail - vertexDataPtr_head) / num_floats_per_vertex;
    unsigned int numIndices = mesh.getNumVertices();

    memcpy(vertexDataPtr_tail, mesh.getVertexDataPtr(), mesh.getNumVertexBytes());
    vertexDataPtr_tail += mesh.getNumVertexBytes() / sizeof(float);

    memcpy(normalDataPtr_tail, mesh.getNormalDataPtr(), mesh.getNumNormalBytes());
    normalDataPtr_tail += mesh.getNumNormalBytes() / sizeof(float);

    batchInfoVec.at(meshNumber) = BatchInfo(startIndex, numIndices);
}

//----------------------------------------------------------------------------------------
/**
 *
 * @param outVector
 */
void MeshConsolidator::getBatchInfo(vector<BatchInfo> & outVector) const {
    for(const BatchInfo &b : this->batchInfoVec) {
        outVector.push_back(b);
    }
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return
 */
const float * MeshConsolidator::getVertexDataPtr() const {
    return vertexDataPtr_head;
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return
 */
const float * MeshConsolidator::getNormalDataPtr() const {
    return normalDataPtr_head;
}

//----------------------------------------------------------------------------------------
/**
 *
 */
unsigned long MeshConsolidator::getNumVertexBytes() const {
    return totalVertexBytes;
}

//----------------------------------------------------------------------------------------
/**
 *
 */
unsigned long MeshConsolidator::getNumNormalBytes() const {
    return totalNormalBytes;
}
