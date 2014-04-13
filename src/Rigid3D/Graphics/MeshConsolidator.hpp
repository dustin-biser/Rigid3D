#ifndef RIGID3D_MESH_CONSOLIDATOR_HPP_
#define RIGID3D_MESH_CONSOLIDATOR_HPP_

#include <Rigid3D/Graphics/Mesh.hpp>

#include <initializer_list>
#include <utility>
#include <memory>
#include <unordered_map>

namespace Rigid3D {
    
    typedef  const char *  MeshID;
    typedef  const char *  ObjFile;

    /**
     * Datastructure for specifying the layout of a \c Mesh object's data
     * elements (e.g. vertices and normals) within a contiguous memory block.
     *
     * Helpful when doing batch rendering such as the following OpenGL example that
     * demonstrates rendering a single triangle:
     * \code{.cpp}
     *  BatchInfo batchInfo(0, 3);
     *  glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
     * \endcode
     *
     */
    struct BatchInfo {
        unsigned int startIndex;
        unsigned int numIndices;

        BatchInfo()
                : startIndex(0), numIndices(0) { }

        BatchInfo(unsigned int startIndex, unsigned int numIndices)
                : startIndex(startIndex), numIndices(numIndices) { }

        BatchInfo(const BatchInfo & other)
                : startIndex(other.startIndex), numIndices(other.numIndices) { }
    };

    /**
     * @brief Class for consolidating \c Mesh attribute data into contiguous blocks of memory.
     *
     * For each \c Mesh data attribute (e.g. vertices, normals, ect.) a single
     * contiguous block of memory is allocated.  All Mesh data are then packed
     * into these memory blocks so that there is one block for vertices, one
     * block for normals, and so on.
     *
     * Once all \c Mesh objects have been consolidated, data for any particular
     * \c Mesh can then be accessed through a combination of attribute data
     * pointers, such as calling \c MeshConsolidator::getVertexDataPtr(), and the
     * use \c BatchInfo objects, which specify the starting index and number of
     * indices for the \c Mesh's data.
     *
     * \c The MeshConsolidator class was designed to make rendering multiple \c Mesh objects
     * simple.  Below is a code sample of how to use the class for rendering in an OpenGL program.
     *
     * \code{.cpp}
     *  MeshConsolidator meshConsolidator = {"cube.obj", "torus.obj", "sphere.obj"};
     *  // Register vertex positions with OpenGL within the context of the bound VAO.
     *  glGenBuffers(1, &vbo_vertices);
     *  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
     *  glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexBytes(), meshConsolidator.getVertexDataPtr(), GL_STATIC_DRAW);
     *
     *  // Acquire BatchInfos for each Mesh.
     *  vector<BatchInfo> batchInfoVec;
     *  meshConsolidator.getBatchInfo(batchInfoVec);
     *
     *  // Render all Meshes
     *  for(BatchInfo batch : batchInfoVec) {
     *      glDrawArrays(GL_TRIANGLES, batch.startIndex, batch.numIndices);
     *  }
     * \endcode
     *
     * @see BatchInfo
     * @see Mesh
     */
    class MeshConsolidator {
    public:
        MeshConsolidator();

        MeshConsolidator(std::initializer_list<std::pair<MeshID, const Mesh *> > list);

        MeshConsolidator(std::initializer_list<std::pair<MeshID, ObjFile> > list);

        ~MeshConsolidator();

        const float * getVertexPositionDataPtr() const;

        const float * getVertexNormalDataPtr() const;

        unsigned long getNumVertexPositionBytes() const;

        unsigned long getNumVertexNormalBytes() const;

        void getBatchInfo(std::unordered_map<const char *, BatchInfo> & batchInfoMap) const;

    private:
        void processMeshes(const std::unordered_map<MeshID, const Mesh *> & meshMap);

        void consolidateMesh(MeshID meshId, const Mesh & mesh);

        unsigned long totalPositionBytes;
        unsigned long totalNormalBytes;

        std::shared_ptr<float> vertexPositionDataPtr_head;
        float * vertexPositionDataPtr_tail;

        std::shared_ptr<float> normalDataPtr_head;
        float * normalDataPtr_tail;

        std::unordered_map<MeshID, BatchInfo> batchInfoMap;

        static const short num_floats_per_vertex = 3;
    };

} // end namespace GlUtils

#endif /* RIGID3D_MESH_CONSOLIDATOR_HPP_ */
