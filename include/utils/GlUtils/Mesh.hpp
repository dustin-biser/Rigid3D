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
    private:
        vector<glm::vec4> glm_vertices;
        vector<glm::vec3> glm_normals;

        vector<float> vertices;
        vector<float>  normals;
        vector<unsigned short>  indices;  /**< Collection of vertex index elements */

    public:
        Mesh(const char* objFileName);

        Mesh();

        ~Mesh();

        void fromObjFile(const char* objFileName);

        const float * getVertexDataPtr();

        const float * getNormalDataPtr();

        const unsigned short * getIndexDataPtr();

        size_t getVertexDataBytes();

        size_t getNormalDataBytes();

        size_t getIndexDataBytes();

    private:
        void initialize();
        void loadFromObjFile(const char* objFileName);
    };
}

#endif /* MESH_HPP_ */
