#include "Mesh.hpp"

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/regex.hpp>

namespace Rigid3D {

using std::ifstream;
using std::istringstream;
using std::stringstream;
using std::endl;
using boost::regex;
using boost::regex_match;
using boost::smatch;

//----------------------------------------------------------------------------------------
/**
 * Constructs a Mesh object from a Wavefront .obj file format.
 *
 * @param objFileName - path to .obj file
 */
Mesh::Mesh(const char * objFileName) {
    loadFromObjFile(objFileName);
}

//----------------------------------------------------------------------------------------
Mesh::Mesh() {

}

//----------------------------------------------------------------------------------------
Mesh::~Mesh() {

}

//----------------------------------------------------------------------------------------
void Mesh::loadFromObjFile(const char * objFileName){
    // Reset datastructures before loading them with data.  Useful if new .obj
    // file contains a mesh with less vertex/normal data than previously parsed file.
    vertexPositions.resize(0);
    vertexNormals.resize(0);
    textureCoords.resize(0);

    ifstream in(objFileName, std::ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFileName
                     << " within method Mesh::loadFromObjFile." << endl;
        throw Rigid3DException(errorMessage.str().c_str());
    }

    string line;
    int vertexIndexA, vertexIndexB, vertexIndexC;
    int normalIndexA, normalIndexB, normalIndexC;
    int textureCoordIndexA, textureCoordIndexB, textureCoordIndexC;
    vector<vec3> tmp_vertices;
    vector<vec3> tmp_normals;
    vector<vec2> tmp_textureCoords;

    // Regex pattern of face indices line containing no texture coordinate indices.
    regex pattern("^f (\\d+/\\d+/\\d+ ){2}\\d+/\\d+/\\d+$");
    smatch match;

    while (!in.eof()) {
        try {
            getline(in, line);
        } catch(const ifstream::failure & e) {
            in.close();
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw Rigid3DException(errorMessage.str().c_str());
        }

        if (line.substr(0,2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            glm::vec3 vertex; s >> vertex.x; s >> vertex.y; s >> vertex.z;
            tmp_vertices.push_back(vertex);

        } else if (line.substr(0,3) == "vn ") {
            // Normal data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            vec3 normal; s >> normal.x; s >> normal.y; s >> normal.z;
            tmp_normals.push_back(normal);

        } else if (line.substr(0,3) == "vt ") {
            // Texture coordinate data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            vec2 textureCoord; s >> textureCoord.s; s >> textureCoord.t;
            tmp_textureCoords.push_back(textureCoord);

        } else if (line.substr(0, 2) == "f ") {
            // Face index data on this line.

            if (regex_match(line, match, pattern)) {
                // Line contains vertex, texture, and normal indices.
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertexIndexA, &textureCoordIndexA, &normalIndexA,
                        &vertexIndexB, &textureCoordIndexB, &normalIndexB,
                        &vertexIndexC, &textureCoordIndexC, &normalIndexC);

                // .obj file uses indices that start at 1, so subtract 1 so they start at 0.
                textureCoordIndexA--;
                textureCoordIndexB--;
                textureCoordIndexC--;

                textureCoords.push_back(tmp_textureCoords[textureCoordIndexA]);
                textureCoords.push_back(tmp_textureCoords[textureCoordIndexB]);
                textureCoords.push_back(tmp_textureCoords[textureCoordIndexC]);

            } else {
                sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexIndexA, &normalIndexA,
                                                               &vertexIndexB, &normalIndexB,
                                                               &vertexIndexC, &normalIndexC);
            }

            vertexIndexA--;
            vertexIndexB--;
            vertexIndexC--;
            normalIndexA--;
            normalIndexB--;
            normalIndexC--;

            vertexPositions.push_back(tmp_vertices[vertexIndexA]);
            vertexPositions.push_back(tmp_vertices[vertexIndexB]);
            vertexPositions.push_back(tmp_vertices[vertexIndexC]);

            vertexNormals.push_back(tmp_normals[normalIndexA]);
            vertexNormals.push_back(tmp_normals[normalIndexB]);
            vertexNormals.push_back(tmp_normals[normalIndexC]);
        }
    }

    in.close();
}

//----------------------------------------------------------------------------------------
/**
 * Initialize this Mesh object using contents of the given Wavefront .obj file.
 *
 * @param objFileName - path to .obj file
 */
void Mesh::fromObjFile(const char * objFileName) {
    loadFromObjFile(objFileName);
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
    return vertexPositions.size();
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of normals for this \c Mesh, where each normal is
 * composed of 3 floats {x,y,z}.
 */
unsigned int Mesh::getNumVertexNormals() const {
   return vertexNormals.size();
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of texture coordinates for this \c Mesh, where each texture coordinate is
 * composed of 2 floats {s,t}.
 */
unsigned int Mesh::getNumTextureCoords() const {
    return textureCoords.size();
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
