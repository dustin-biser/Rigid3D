#include "Transform.hpp"

namespace Rigid3D {

//----------------------------------------------------------------------------------------
Transform::Transform() {

}

//----------------------------------------------------------------------------------------
Transform::Transform(const vec3 & position, const quat & pose)
    : position(position),
      pose(pose) {

}

//----------------------------------------------------------------------------------------
Transform::~Transform() {

}

}
