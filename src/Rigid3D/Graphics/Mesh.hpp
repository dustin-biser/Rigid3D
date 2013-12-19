#ifndef RIGID3D_MESH_HPP_
#define RIGID3D_MESH_HPP_

#include <Rigid3D/Common/Settings.hpp>

#include <vector>
#include <string>

namespace Rigid3D {

using std::vector;
using std::string;

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
        const vector<vec3> * getVertexPositionVector() const;

        const float * getVertexNormalDataPtr() const;
        const vector<vec3> * getVertexNormalVector() const;

        const float * getTextureCoordDataPtr() const;
        const vector<vec2> * getTextureCoordVector() const;

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

#endif /* RIGID3D_MESH_HPP_ */
