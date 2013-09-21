#include <MeshConsolidator.hpp>
#include <GlUtilsException.hpp>
#include <cstring>
#include <cstdlib>
#include <Mesh.hpp>

using namespace GlUtils;

//----------------------------------------------------------------------------------------
/**
 * Default constructor
 */
MeshConsolidator::MeshConsolidator()
        : totalVertexBytes(0),
          totalNormalBytes(0),
          vertexDataPtr_head(nullptr),
          vertexDataPtr_tail(nullptr),
          normalDataPtr_head(nullptr),
          normalDataPtr_tail(nullptr) { }


//----------------------------------------------------------------------------------------
/**
 * Constructs a \c MeshConsolidator object from a list of \c const \c Mesh pointers.
 * @param meshList
 */
MeshConsolidator::MeshConsolidator(initializer_list<const Mesh *> meshList)
        : totalVertexBytes(0), totalNormalBytes(0) {

    vector<const Mesh *> meshVector = meshList;
    processMeshes(meshVector);
}

//----------------------------------------------------------------------------------------
/**
 * Constructs a \c MeshConsolidator object from a list of \c Wavefront \c .obj file names.
 * @param fileNameList
 */
MeshConsolidator::MeshConsolidator(initializer_list<const char *> fileNameList)
        : totalVertexBytes(0), totalNormalBytes(0) {

    // Meshes will auto-destruct at the end of this method when vector goes out of scope.
    vector<shared_ptr<Mesh> > meshVector;
    meshVector.resize(fileNameList.size());

    // Use Mesh instances to process each .obj file one at a time.
    unsigned int i = 0;
    for(const char * fileName : fileNameList) {
        meshVector.at(i) = make_shared<Mesh>(fileName);
        i++;
    }

    vector<const Mesh *> readyMeshes;
    for(shared_ptr<Mesh> meshPtr : meshVector) {
        readyMeshes.push_back(meshPtr.get());
    }

    processMeshes(readyMeshes);
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::processMeshes(const vector<const Mesh *> & meshVector) {
    batchInfoVec.resize(meshVector.size());

    for(const Mesh * m : meshVector) {
        totalVertexBytes += m->getNumVertexBytes();
        totalNormalBytes += m->getNumNormalBytes();
    }

    vertexDataPtr_head = shared_ptr<float>((float *)malloc(totalVertexBytes), free);
    if (vertexDataPtr_head.get() == (float *)0) {
        throw GlUtilsException("Unable to allocate system memory within class MeshConsolidator::processMeshes()");
    }

    normalDataPtr_head = shared_ptr<float>((float *)malloc(totalNormalBytes), free);
    if (vertexDataPtr_head.get() == (float *)0) {
        throw GlUtilsException("Unable to allocate system memory within class MeshConsolidator::processMeshes()");
    }

    // Assign pointers to beginning of memory blocks.
    vertexDataPtr_tail = vertexDataPtr_head.get();
    normalDataPtr_tail = normalDataPtr_head.get();

    unsigned int meshNumber = 0;
    for(const Mesh * m : meshVector) {
        consolidateMesh(*m, meshNumber);
        meshNumber++;
    }
}

//----------------------------------------------------------------------------------------
MeshConsolidator::~MeshConsolidator() {
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::consolidateMesh(const Mesh & mesh, unsigned int meshNumber) {
    unsigned int startIndex = (vertexDataPtr_tail - vertexDataPtr_head.get()) / num_floats_per_vertex;
    unsigned int numIndices = mesh.getNumVertices();

    memcpy(vertexDataPtr_tail, mesh.getVertexDataPtr(), mesh.getNumVertexBytes());
    vertexDataPtr_tail += mesh.getNumVertexBytes() / sizeof(float);

    memcpy(normalDataPtr_tail, mesh.getNormalDataPtr(), mesh.getNumNormalBytes());
    normalDataPtr_tail += mesh.getNumNormalBytes() / sizeof(float);

    batchInfoVec.at(meshNumber) = BatchInfo(startIndex, numIndices);
}

//----------------------------------------------------------------------------------------
/**
 * Appends to the back of \c outVector a series of \c BatchInfo objects that
 * specify the starting index location of each Mesh object's data (e.g. vertices
 * and normals) within contiguous memory, and the number of data elements each
 * Mesh is composed of.
 *
 * Each \c BatchInfo object corresponds to the data layout for a single \c Mesh
 * object, and the order of the appended \c BatchInfo objects matches the order of
 * the \c MeshConsolidator constructor arguments.
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
 * @return the starting memory location for all consolidated \c Mesh vertex data.
 */
const float * MeshConsolidator::getVertexDataPtr() const {
    return vertexDataPtr_head.get();
}

//----------------------------------------------------------------------------------------
/**
 * @return the starting memory location for all consolidated \c Mesh normal data.
 */
const float * MeshConsolidator::getNormalDataPtr() const {
    return normalDataPtr_head.get();
}

//----------------------------------------------------------------------------------------
/**
 * @return the total number of bytes of all consolidated \c Mesh vertex data.
 */
unsigned long MeshConsolidator::getNumVertexBytes() const {
    return totalVertexBytes;
}

//----------------------------------------------------------------------------------------
/**
 * @return the total number of bytes of all consolidated \c Mesh normal data.
 */
unsigned long MeshConsolidator::getNumNormalBytes() const {
    return totalNormalBytes;
}
