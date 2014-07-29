#include "Mesh.hpp"

#include <Rigid3D/Common/Rigid3DException.hpp>
#include <Rigid3D/Graphics/ObjFileLoader.hpp>

#include <utility>

namespace Rigid3D {

//----------------------------------------------------------------------------------------
/**
 * Constructs a Mesh object from a Wavefront .obj file format.
 *
 * @param objFileName - path to .obj file
 */
Mesh::Mesh(const char * objFileName) {
    ObjFileLoader::decode(objFileName,
                          this->vertexPositions,
                          this->vertexNormals,
                          this->textureCoords);
}

//----------------------------------------------------------------------------------------
Mesh::Mesh() {
    // Empty, like my ice cold heart.
}

//----------------------------------------------------------------------------------------
/**
* Move assignment operator.
*/
Mesh & Mesh::operator = (Mesh && other) {
    this->vertexPositions = std::move(other.vertexPositions);
    this->vertexNormals = std::move(other.vertexNormals);
    this->textureCoords = std::move(other.textureCoords);

    return *this;
}

//----------------------------------------------------------------------------------------
const float * Mesh::getVertexPositionDataPtr() const {
    // Return the first float within the first vec3 of the vertices vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((vertexPositions.data())->x));
}

//----------------------------------------------------------------------------------------
const vector<vec3> * Mesh::getVertexPositionVector() const {
   return &vertexPositions;
}

//----------------------------------------------------------------------------------------
const float * Mesh::getVertexNormalDataPtr() const {
    // Return the first float within the first vec3 of the normals vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((vertexNormals.data())->x));
}

//----------------------------------------------------------------------------------------
const vector<vec3> * Mesh::getVertexNormalVector() const {
    return &vertexNormals;
}

//----------------------------------------------------------------------------------------
const float* Mesh::getTextureCoordDataPtr() const {
    // Return the first float within the first vec2 of the textureCoords vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((textureCoords.data())->s));
}

//----------------------------------------------------------------------------------------
const vector<vec2> * Mesh::getTextureCoordVector() const {
    return &textureCoords;
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's vertex data.
 *
 * @return size_t
 */
size_t Mesh::getNumVertexPositionBytes() const {
    return vertexPositions.size() * num_elements_per_vertex_position * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's normals.
 *
 * @return size_t
 */
size_t Mesh::getNumVertexNormalBytes() const {
    return vertexNormals.size() * num_elements_per_vertex_normal * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's texture coordinate data.
 *
 * @return size_t
 */
size_t Mesh::getNumTextureCoordBytes() const {
    return textureCoords.size() * num_elements_per_texturedCoord * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of vertices for this \c Mesh, where each vertex is
 * composed of 3 floats {x,y,z}.
 */
unsigned int Mesh::getNumVertexPositions() const {
    return (unsigned int)(vertexPositions.size());
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of normals for this \c Mesh, where each normal is
 * composed of 3 floats {x,y,z}.
 */
unsigned int Mesh::getNumVertexNormals() const {
   return (unsigned int)(vertexNormals.size());
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of texture coordinates for this \c Mesh, where each texture coordinate is
 * composed of 2 floats {s,t}.
 */
unsigned int Mesh::getNumTextureCoords() const {
    return (unsigned int)(textureCoords.size());
}

//----------------------------------------------------------------------------------------
unsigned int Mesh::getNumElementsPerVertexPosition() const {
    return num_elements_per_vertex_position;
}

//----------------------------------------------------------------------------------------
unsigned int Mesh::getNumElementsPerVertexNormal() const {
    return num_elements_per_vertex_normal;
}

//----------------------------------------------------------------------------------------
unsigned int Mesh::getNumElementsPerTextureCoord() const {
    return num_elements_per_texturedCoord;
}

} // end namespace GlUtils
