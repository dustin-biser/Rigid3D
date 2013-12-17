#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <vector>
using std::vector;

#include "glm/glm.hpp"
using glm::vec3;

namespace Rigid3D {

    class Geometry {
    public:
        Geometry();

        Geometry(const vector<vec3> * vertexPositions);

        const vector<vec3> * getVertexPositions() const;

        vec3 getCenter() const;

        vec3 getMinExtension() const;

        vec3 getMaxExtension() const;

    private:
        const vector<vec3> * vertexPositions;
        vec3 center; // Geometric center given in model space coordinates.
        vec3 min;  // Minimum extensions along x,y,z directions in model space.
        vec3 max;  // Maximum extensions along x,y,z directions in model space.
    };
}

#endif /* GEOMETRY_HPP_ */
