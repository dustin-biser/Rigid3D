/*
 * Mesh.cpp
 *
 *  Created on: Jul 6, 2013
 *      Author: Dustin Biser
 */

#include <Mesh.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace GlUtils {

    Mesh::Mesh(const char* objFileName) {
        initialize();
        load_objFile(objFileName);
    }

    Mesh::Mesh() {
        initialize();
    }

    void Mesh::initialize() {
        // Allocate a new vector for vertices, normals, and elements to be owned
        // by their respective shared_ptrs.
        this->vertices = make_shared<vector<glm::vec4> >();
        this->normals = make_shared<vector<glm::vec3> >();
        this->elements = make_shared<vector<unsigned short> >();
    }

    Mesh::~Mesh() {
        // Relinquish ownership of objects owned by member shared_ptrs.
        this->vertices.reset();
        this->normals.reset();
        this->elements.reset();
    }

    void Mesh::load_objFile(const char* objFileName){
        ifstream in(objFileName, ios::in);
        if (!in) {
            cerr << "Unable to open .obj file: " << objFileName << endl;
            exit(1);
        }

        string line;
        while (getline(in, line)) {
            if (line.substr(0,2) == "v ") {
                // Vertex data on this line.
                // Get entire line excluding first 2 chars.
                istringstream s(line.substr(2));
                // Allocate new vec4, fill in its elements, and add to vertices vector.
                glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
                this->vertices->push_back(v);
            }
            else if (line.substr(0,2) == "f ") {
                // Face data on this line.
                istringstream s(line.substr(2));
                unsigned short a, b, c;
                s >> a; s >> b; s >> c;
                a--; b--; c--;
                this->elements->push_back(a);
                this->elements->push_back(b);
                this->elements->push_back(c);
            }
            else if (line[0] == '#' ) { /* ignoring this line */ }
            else { /* ignoring this line */ }
        }

        // Construct a normal for each vertex.
        this->normals->resize(this->vertices->size(), glm::vec3(0.0, 0.0, 0.0));
        for (unsigned int i = 0; i < this->elements->size(); i += 3) {
            unsigned short ia = this->elements->at(i);
            unsigned short ib = this->elements->at(i + 1);
            unsigned short ic = this->elements->at(i + 2);

            // Create two vectors; one from vertexA to vertexB, and one from vertexA to vertexC.
            // Compute normal by taking cross produce of these two vectors.  This is the normal
            // for the three vertices.
            glm::vec3 normal = glm::normalize(glm::cross(
                    glm::vec3( this->vertices->at(ib) ) - glm::vec3( this->vertices->at(ia) ),
                    glm::vec3( this->vertices->at(ic) ) - glm::vec3( this->vertices->at(ia) )
            ));

            this->normals->at(ia) = this->normals->at(ib) = this->normals->at(ic) = normal;
        }
    }

    void Mesh::fromObjFile(const char* objFileName) {
        load_objFile(objFileName);
    }

    glm::vec4 * Mesh::getVertices() {
        return this->vertices->data();
    }

    glm::vec3 * Mesh::getNormals() {
        return this->normals->data();
    }

    unsigned short * Mesh::getElements() {
        return this->elements->data();
    }

    size_t Mesh::getVertexDataBytes() {
        return this->vertices->size() * sizeof(glm::vec4);
    }

    size_t Mesh::getNormalDataBytes() {
        return this->normals->size() * sizeof(glm::vec3);
    }

} // end namespace GlUtils



