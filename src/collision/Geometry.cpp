#include "Geometry.hpp"

#include <cfloat>
#include <algorithm>

namespace Rigid3D {

//----------------------------------------------------------------------------------------
Geometry::Geometry()
    : vertexPositions(nullptr) {

}

//----------------------------------------------------------------------------------------
Geometry::Geometry(const vector<vec3> * vertexPositions) {
    this->vertexPositions = vertexPositions;

    min = vec3(FLT_MAX);
    max = vec3(FLT_MIN);
    vec3 sum;

    unsigned int count = 0;

    // Compute min and max extensions in local coordinates.
    for (const vec3 & position : *vertexPositions) {
        sum += position;

        min.x = std::min(position.x, min.x);
        min.y = std::min(position.y, min.y);
        min.z = std::min(position.z, min.z);

        max.x = std::max(position.x, max.x);
        max.y = std::max(position.y, max.y);
        max.z = std::max(position.z, max.z);

        count++;
    }

    // Compute center of geometry in local coordinates.
    if (count > 0) {
        center = sum * (1.0f / count);
    }
}

//----------------------------------------------------------------------------------------
const vector<vec3> * Geometry::getVertexPositions() const {
    return vertexPositions;
}

//----------------------------------------------------------------------------------------
vec3 Geometry::getCenter() const {
    return center;
}

//----------------------------------------------------------------------------------------
vec3 Geometry::getMinExtension() const {
    return min;
}

//----------------------------------------------------------------------------------------
vec3 Geometry::getMaxExtension() const {
    return max;
}

} // end namespace Rigid3D
