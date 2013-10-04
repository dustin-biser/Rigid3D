/**
 * @brief To aid in printing common \c glm types to designated output streams.
 */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using glm::quat;

#include <iostream>
using std::ostream;
using std::endl;

#include <iomanip>
using std::fixed;
using std::setprecision;

#define GLM_OUT_STREAM_PRECISION 6

namespace glm { namespace detail {

    inline std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
        out << "vec2:[" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
        out << "vec3:[" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y << " "<< vec.z
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
        out << "vec4:[" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y << " "<< vec.z << " " << vec.w
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat2 &mat) {
        out << endl << "mat2:" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << "]" << endl
            << "     [" << mat[0][1] << " " << mat[1][1] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat3 &mat) {
        out << endl << "mat3:" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << "]" << endl
            << "     [" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << "]" << endl
            << "     [" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
        out << endl << "mat4:" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "]" << endl
            << "     [" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "]" << endl
            << "     [" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "]" << endl
            << "     [" << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::quat &q) {
        out << "quat:[" << fixed << setprecision(GLM_OUT_STREAM_PRECISION)
            << q.w << " " << q.x << " "<< q.y << " " << q.z
            << "]";

        return out;
    }

}} // end namespace glm::detail
