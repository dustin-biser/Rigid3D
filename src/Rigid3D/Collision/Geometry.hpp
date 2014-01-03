#ifndef RIGID3D_GEOMETRY_HPP_
#define RIGID3D_GEOMETRY_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    struct Geometry {
        vec3 * positionSet;
        vec3 * normalSet;

        uvec3 * positionIndices;
        uvec3 * normalIndices;

        uint16 numPositions;
        uint16 numNormals;
        uint16 numIndices;
    };
}

#endif /* RIGID3D_GEOMETRY_HPP_ */
