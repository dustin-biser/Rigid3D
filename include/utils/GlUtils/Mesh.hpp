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
        vector<vec3> vertexPositions;
        static const short num_elements_per_vertex_position = 3;

        vector<vec3>  vertexNormals;
        static const short num_elements_per_vertex_normal = 3;

        vector<vec2> textureCoords;
        static const short num_elements_per_texturedCoord = 2;

    public:
        Mesh(const char * objFileName);

        Mesh();

        virtual ~Mesh();

        void fromObjFile(const char * objFileName);

        const float * getVertexPositionDataPtr() const;

        const float * getVertexNormalDataPtr() const;

        const float * getTextureCoordDataPtr() const;

        size_t getNumVertexPositionBytes() const;

        size_t getNumVertexNormalBytes() const;

        size_t getNumTextureCoordBytes() const;

        unsigned int getNumVertexPositions() const;

        unsigned int getNumVertexNormals() const;

        unsigned int getNumTextureCoords() const;

        unsigned int getNumElementsPerVertexPosition() const;

        unsigned int getNumElementsPerVertexNormal() const;

        unsigned int getNumElementsPerTextureCoord() const;

    private:
        void loadFromObjFile(const char * objFileName);
    };
}

#endif /* MESH_HPP_ */
