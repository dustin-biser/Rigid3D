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
Mesh::Mesh(const char * objFileName) {
    loadFromObjFile(objFileName);
}

//----------------------------------------------------------------------------------------
void Mesh::loadFromObjFile(const char * objFileName){
    // Reset datastructures before loading them with data.  Useful if new .obj
    // file contains a mesh with less vertex/normal data than previously parsed file.
    vertices.resize(0);
    normals.resize(0);

    ifstream in(objFileName, ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFileName
                     << " within method Mesh::loadFromObjFile." << endl;
        throw GlUtilsException(errorMessage.str());
    }

    string line;
    int vertexIndexA, vertexIndexB, vertexIndexC;
    int normalIndexA, normalIndexB, normalIndexC;
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
            sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexIndexA, &normalIndexA,
                                                           &vertexIndexB, &normalIndexB,
                                                           &vertexIndexC, &normalIndexC);

            // .obj file uses indices that start at 1, so subtract 1 so they start at 0.
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
