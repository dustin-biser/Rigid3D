#include <Mesh.hpp>

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::stringstream;

#include <iostream>
using std::endl;

#include <boost/regex.hpp>
using boost::regex;
using boost::regex_match;
using boost::smatch;

#include <GlUtilsException.hpp>

namespace GlUtils {

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
void Mesh::loadFromObjFile(const char * objFileName){
    // Reset datastructures before loading them with data.  Useful if new .obj
    // file contains a mesh with less vertex/normal data than previously parsed file.
    vertices.resize(0);
    normals.resize(0);
    textureCoords.resize(0);

    ifstream in(objFileName, std::ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFileName
                     << " within method Mesh::loadFromObjFile." << endl;
        throw GlUtilsException(errorMessage.str().c_str());
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
            throw GlUtilsException(errorMessage.str().c_str());
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

            vertices.push_back(tmp_vertices[vertexIndexA]);
            vertices.push_back(tmp_vertices[vertexIndexB]);
            vertices.push_back(tmp_vertices[vertexIndexC]);

            normals.push_back(tmp_normals[normalIndexA]);
            normals.push_back(tmp_normals[normalIndexB]);
            normals.push_back(tmp_normals[normalIndexC]);
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
const float * Mesh::getVertexDataPtr() const {
    // Return the first float within the first vec3 of the vertices vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((vertices.data())->x));
}

//----------------------------------------------------------------------------------------
const float * Mesh::getNormalDataPtr() const {
    // Return the first float within the first vec3 of the normals vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((normals.data())->x));
}

//----------------------------------------------------------------------------------------
const float* Mesh::getTextureCoordDataPtr() const {
    // Return the first float within the first vec2 of the textureCoords vector.  All
    // data is contiguous in memory.
    return const_cast<float *>(&((textureCoords.data())->s));
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's vertex data.
 *
 * @return size_t
 */
size_t Mesh::getNumVertexBytes() const {
    return vertices.size() * num_elements_per_vertex * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's normals.
 *
 * @return size_t
 */
size_t Mesh::getNumNormalBytes() const {
    return normals.size() * num_elements_per_normal * sizeof(float);
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
unsigned int Mesh::getNumVertices() const {
    return vertices.size();
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of normals for this \c Mesh, where each normal is
 * composed of 3 floats {x,y,z}.
 */
unsigned int Mesh::getNumNormals() const {
   return normals.size();
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

} // end namespace GlUtils
