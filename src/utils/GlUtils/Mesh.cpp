#include <Mesh.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GlUtilsException.hpp>

using namespace GlUtils;
using namespace std;

//----------------------------------------------------------------------------------------
/**
 * Constructs a Mesh object from a Wavefront .obj file format.
 *
 * @param objFileName - path to .obj file
 */
Mesh::Mesh(const char* objFileName) {
    loadFromObjFile(objFileName);
}

//----------------------------------------------------------------------------------------
void Mesh::loadFromObjFile(const char* objFileName){
    // Reset datastructures before loading them with data.
    vertices.resize(0);
    normals.resize(0);
    glm_vertices.resize(0);
    glm_normals.resize(0);

    ifstream in(objFileName, ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFileName << endl;
        throw GlUtilsException(errorMessage.str());
    }

    string line;
    int vertexIndexA, vertexIndexB, vertexIndexC, normalIndex;
    vector<vec3> tmp_normals;
    vector<vec3> tmp_vertices;

    while (!in.eof()) {
        try {
            getline(in, line);
        } catch(const ifstream::failure & e) {
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw GlUtilsException(errorMessage.str());
        }

        if (line.substr(0,2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            glm::vec3 vertex; s >> vertex.x; s >> vertex.y; s >> vertex.z;
            tmp_vertices.push_back(vertex);
        }
        else if (line.substr(0,3) == "vn ") {
            // Normal data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            vec3 normal; s >> normal.x; s >> normal.y; s >> normal.z;
            tmp_normals.push_back(normal);
        }
        else if (line.substr(0, 2) == "f ") {
            // Get vertex indices and which normal to assign to those vertices.
            sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexIndexA, &normalIndex,
                                                           &vertexIndexB, &normalIndex,
                                                           &vertexIndexC, &normalIndex);

            // .obj file uses indices that start at 1, so subtract one so they start at 0.
            vertexIndexA--;
            vertexIndexB--;
            vertexIndexC--;
            normalIndex--;

            glm_vertices.push_back(tmp_vertices[vertexIndexA]);
            glm_vertices.push_back(tmp_vertices[vertexIndexB]);
            glm_vertices.push_back(tmp_vertices[vertexIndexC]);

            glm_normals.push_back(tmp_normals[normalIndex]);
            glm_normals.push_back(tmp_normals[normalIndex]);
            glm_normals.push_back(tmp_normals[normalIndex]);
        }
    }

    // Copy data to vertex vector
    for(glm::vec3 v : glm_vertices) {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);
    }

    // Copy data to normal vector
    for(glm::vec3 v : glm_normals) {
        normals.push_back(v.x);
        normals.push_back(v.y);
        normals.push_back(v.z);
    }

    // Clear data from glm_vertices and glm_normals, no longer needed.
    glm_vertices.resize(0);
    glm_normals.resize(0);
}

//----------------------------------------------------------------------------------------
/**
 * Initialize this Mesh object using contents of the given Wavefront .obj file.
 *
 * @param objFileName - path to .obj file
 */
void Mesh::fromObjFile(const char* objFileName) {
    loadFromObjFile(objFileName);
}

//----------------------------------------------------------------------------------------
const float * Mesh::getVertexDataPtr() const {
    return const_cast<float *>(vertices.data());
}

//----------------------------------------------------------------------------------------
const float * Mesh::getNormalDataPtr() const {
    return const_cast<float *>(normals.data());
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's vertex data.
 *
 * @return size_t
 */
size_t Mesh::getNumVertexBytes() const {
    return vertices.size() * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 * Returns the total size in bytes of the Mesh's normals.
 *
 * @return size_t
 */
size_t Mesh::getNumNormalBytes() const {
    return normals.size() * sizeof(float);
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of vertices for this \c Mesh, where each vertex is
 * composed of 3 floats {x,y,z}.
 */
size_t Mesh::getNumVertices() const {
    return (size_t)(vertices.size() / 3.0f);
}

//----------------------------------------------------------------------------------------
/**
 *
 * @return the number of normals for this \c Mesh, where each normal is
 * composed of 3 floats {x,y,z}.
 */
size_t Mesh::getNumNormals() const {
   return (size_t)(normals.size() / 3.0f);
}
