#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm/glm.hpp>
using glm::vec3;

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;

namespace GlUtils {

    class Mesh {
    private:
        vector<vec3> vertices;
        static const short num_elements_per_vertex = 3;

        vector<vec3>  normals;
        static const short num_elements_per_normal = 3;

    public:
        Mesh(const char * objFileName);

        Mesh() { }

        ~Mesh() { }

        void fromObjFile(const char * objFileName);

        const float * getVertexDataPtr() const;

        const float * getNormalDataPtr() const;

        size_t getNumVertexBytes() const;

        size_t getNumNormalBytes() const;

        unsigned int getNumVertices() const;

        unsigned int getNumNormals() const;


    private:
        void loadFromObjFile(const char * objFileName);
    };
}

#endif /* MESH_HPP_ */
