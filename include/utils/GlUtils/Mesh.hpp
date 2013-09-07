#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm/glm.hpp>
#include <vector>
#include <memory>

using namespace std;
using namespace glm;

namespace GlUtils {

    class Mesh {
    private:
        vector<float> vertices;
        vector<float>  normals;

        vector<vec3> glm_vertices;
        vector<vec3> glm_normals;

    public:
        Mesh(const char* objFileName);

        Mesh() { }

        ~Mesh() { }

        void fromObjFile(const char* objFileName);

        const float * getVertexDataPtr() const;

        const float * getNormalDataPtr() const;

        size_t getNumVertexBytes() const;

        size_t getNumNormalBytes() const;

        size_t getNumVertices() const;

        size_t getNumNormals() const;


    private:
        void loadFromObjFile(const char* objFileName);
    };
}

#endif /* MESH_HPP_ */
