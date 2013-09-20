#ifndef MESH_CONSOLIDATOR_HPP_
#define MESH_CONSOLIDATOR_HPP_

#include <Mesh.hpp>
#include <initializer_list>
#include <vector>
#include <glm/glm.hpp>

using std::initializer_list;
using std::vector;

namespace GlUtils {

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

    class MeshConsolidator {
    public:
        MeshConsolidator(initializer_list<Mesh> meshList);

        ~MeshConsolidator();

        const float * getVertexDataPtr() const;

        const float * getNormalDataPtr() const;

        unsigned long getNumVertexBytes() const;

        unsigned long getNumNormalBytes() const;

        void getBatchInfo(vector<BatchInfo> & outVector) const;

    private:
        MeshConsolidator() = delete;

        void consolidateMesh(const Mesh & mesh, unsigned int meshNumber);

        unsigned long totalVertexBytes;
        unsigned long totalNormalBytes;

        float * vertexDataPtr_head = nullptr;
        float * vertexDataPtr_tail = nullptr;
        float * normalDataPtr_head = nullptr;
        float * normalDataPtr_tail = nullptr;

        vector<BatchInfo> batchInfoVec;

        static const short num_floats_per_vertex = 3;
    };

} // end namespace GlUtils

#endif /* MESH_CONSOLIDATOR_HPP_ */
