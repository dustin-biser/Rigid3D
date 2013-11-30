// AABB.cpp
#include "AABB.hpp"
using glm::normalize;

#include "RayCastInput.hpp"
#include "RayCastOutput.hpp"

#include <cfloat>

#include <cmath>
using std::fabs;

#include <algorithm>
using std::swap;

namespace Rigid3D {

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
bool AABB::rayCast(const RayCastInput & input, RayCastOutput & output) {
    // Let ray be parameterized by R(t) = p + t * d
    // with starting point p, and direction d.
    float tmin = 0.0f;
    float tmax = input.maxLength;

    vec3 p0 = input.p0;
    float p[3] = {p0.x, p0.y, p0.z};

    vec3 dir = normalize(input.p1 - p0);
    float d[3] = {dir.x, dir.y, dir.z};

    // Compute ray intersection intervals against plane slab regions defining
    // the AABB.
    // For each slab region in the x, y, and z directions:
    for (int i = 0; i < 3; i++) {
        if (fabs(d[i]) < FLT_EPSILON) {
            // Ray is parallel to slab.  No intersection if ray's origin is not
            // within slab.
            if (p[i] < min[i] || p[i] > max[i]) return false;
        } else {
            // Compute intersection t value of ray with near and far plane of slab.
            float one_over_d = 1.0f / d[i];
            float t1 = (min[i] - p[i]) * one_over_d;
            float t2 = (max[i] - p[i]) * one_over_d;

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
    output.hitPoint = p0 + dir * tmin;
    output.length = tmin;

    return true;
}

}
