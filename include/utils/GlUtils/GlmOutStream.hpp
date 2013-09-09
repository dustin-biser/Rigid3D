/**
 * @brief To aid in printing common GLM types to designated output streams.
 */

#ifndef GLMOUTSTREAM_HPP_
#define GLMOUTSTREAM_HPP_

#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "vec2:[" << fixed << setprecision(4)
        << vec.x << " " << vec.y
        << "]";

    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "vec3:[" << fixed << setprecision(4)
        << vec.x << " " << vec.y << " "<< vec.z
        << "]";

    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "vec4:[" << fixed << setprecision(4)
        << vec.x << " " << vec.y << " "<< vec.z << " " << vec.w
        << "]";

    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat2 &mat) {
    out << "mat2:" << fixed << setprecision(4)
        << "["      << mat[0][0] << " " << mat[0][1] << "]" << endl
        << "     [" << mat[1][0] << " " << mat[1][1] << "]";

    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat3 &mat) {
    out << "mat3:" << fixed << setprecision(4)
        << "["      << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << "]" << endl
        << "     [" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << "]" << endl
        << "     [" << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << "]";

    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
    out << "mat4:" << fixed << setprecision(4)
        << "["      << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << "]" << endl
        << "     [" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << "]" << endl
        << "     [" << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << "]" << endl
        << "     [" << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << "]";

    return out;
}


#endif /* GLMOUTSTREAM_HPP_ */
