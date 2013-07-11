/*
 * Mesh.hpp
 *
 *  Created on: Jul 6, 2013
 *      Author: Dustin Biser
 */

#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm/glm.hpp>
#include <vector>
#include <memory>

using namespace std;

namespace GlUtils {

    class Mesh {
    public:
        shared_ptr<vector<glm::vec4> > vertices;  /**< Collection of Mesh vertices */
        shared_ptr<vector<glm::vec3> > normals;  /**< Collection of Mesh normals */
        shared_ptr<vector<unsigned short> > elements;  /**< Collection of vertex index elements */

        /**
         * Constructs a Mesh object from a Wavefront .obj file format.
         * @param objFileName - path to .obj file
         */
        Mesh(const char* objFileName);

        /**
         * Default Constructor.
         */
        Mesh();

        ~Mesh();

        /**
         * Initialize this Mesh object using contents of the given Wavefront .obj file.
         * @param objFileName - path to .obj file
         */
        void fromObjFile(const char* objFileName);

        glm::vec4 * getVertices();

        glm::vec3 * getNormals();

        unsigned short * getElements();

        /**
         * Returns the number of bytes composing the Mesh's vertex data.
         * @return size_t
         */
        size_t getVertexDataBytes();

        /**
         * Returns the number of bytes composing the Mesh's normal data.
         * @return size_t
         */
        size_t getNormalDataBytes();

    private:
        void initialize();
        void load_objFile(const char* objFileName);
    };
}

#endif /* MESH_HPP_ */
