#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;

#include <vector>
using std::vector;

#include <string>
using std::string;

namespace GlUtils {

    class Mesh {
    protected:
        vector<vec3> vertices;
        static const short num_elements_per_vertex = 3;

        vector<vec3>  normals;
        static const short num_elements_per_normal = 3;

        vector<vec2> textureCoords;
        static const short num_elements_per_texturedCoord = 2;

    public:
        Mesh(const char * objFileName);

        Mesh() { }

        virtual ~Mesh() { }

        void fromObjFile(const char * objFileName);

        const float * getVertexDataPtr() const;

        const float * getNormalDataPtr() const;

        const float * getTextureCoordDataPtr() const;

        size_t getNumVertexBytes() const;

        size_t getNumNormalBytes() const;

        size_t getNumTextureCoordBytes() const;

        unsigned int getNumVertices() const;

        unsigned int getNumNormals() const;

        unsigned int getNumTextureCoords() const;

    private:
        void loadFromObjFile(const char * objFileName);
    };
}

#endif /* MESH_HPP_ */
