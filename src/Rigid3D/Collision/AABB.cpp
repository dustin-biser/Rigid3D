// AABB.cpp
#include "AABB.hpp"
#include "RayCastInput.hpp"
#include "RayCastOutput.hpp"

#include <cfloat>
#include <cmath>
#include <algorithm>

namespace Rigid3D {

using glm::normalize;
using std::fabs;
using std::swap;

//----------------------------------------------------------------------------------------
/**
 * Performs a ray-cast against this AABB.
 *
 * If ray intersects AABB, 'output' is filled in with information pertaining to
 * the ray intersection.  If ray does not intersect AABB, then data is not
 * written to 'output'.
 *
 * @param input - struct containing ray-cast info.
 * @param output - struct containing ray-cast intersection info if ray hits AABB.
 * @return true if ray intersects AABB, or false otherwise.
 */
bool AABB::rayCast(const RayCastInput & input, RayCastOutput * output) const {
    // Let ray be parameterized by R(t) = p + t * d
    // with starting point p, and direction d.
    float tmin = 0.0f;
    float tmax = input.maxLength;

    vec3 p1 = input.p1;
    float p[3] = {p1.x, p1.y, p1.z};

    vec3 dir = normalize(input.p2 - p1);
    float d[3] = {dir.x, dir.y, dir.z};

    // Compute ray intersection intervals against plane slab regions defining
    // the AABB.
    // For each slab region in the x, y, and z directions:
    for (int i = 0; i < 3; i++) {
        if (fabs(d[i]) < FLT_EPSILON) {
            // Ray is parallel to slab.  No intersection if ray's origin is not
            // within slab.
            if (p[i] < minBounds[i] || p[i] > maxBounds[i]) return false;
        } else {
            // Compute intersection t value of ray with near and far plane of slab.
            float one_over_d = 1.0f / d[i];
            float t1 = (minBounds[i] - p[i]) * one_over_d;
            float t2 = (maxBounds[i] - p[i]) * one_over_d;

            // Make t1 be intersection with near plane, t2 with far plane.
            if (t1 > t2) { swap(t1, t2); }

            // Compute the intersection interval within slab.
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            // Exit with no collision as soon as slab intersection intervals no
            // longer overlap.
            if (tmin > tmax) { return false; }
        }
    }
    // Ray intersects all 3 slabs with overlapping intersection intervals.

    if (output) {
        output->hitPoint = p1 + dir * tmin;
        output->length = tmin;
    }

    return true;
}

//----------------------------------------------------------------------------------------
vec3 AABB::getCenter() const {
    return (minBounds + maxBounds) * 0.5f;
}

} // end namespace Rigid3D
