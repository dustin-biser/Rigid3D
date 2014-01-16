#ifndef RIGID3D_GLM_OUT_STREAM_HPP
#define RIGID3D_GLM_OUT_STREAM_HPP

/**
 * @brief To aid in printing common \c glm types to designated output streams.
 */

#include <Rigid3D/Common/Settings.hpp>

#include <iostream>
#include <iomanip>
#include <vector>

#define GLM_OUT_STREAM_PRECISION 6

namespace glm { namespace detail {

    inline std::ostream &operator<< (std::ostream &out, const glm::uvec2 &uvec) {
        out << "uvec2:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << uvec.x << " " << uvec.y
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::uvec3 &uvec) {
        out << "uvec3:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << uvec.x << " " << uvec.y << " "<< uvec.z
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::uvec4 &uvec) {
        out << "uvec4:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << uvec.x << " " << uvec.y << " "<< uvec.z << " " << uvec.w
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
        out << "vec2:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
        out << "vec3:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y << " "<< vec.z
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
        out << "vec4:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << vec.x << " " << vec.y << " "<< vec.z << " " << vec.w
            << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat2 &mat) {
        out << std::endl << "mat2:" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << "]" << std::endl
            << "     [" << mat[0][1] << " " << mat[1][1] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat3 &mat) {
        out << std::endl << "mat3:" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << "]" << std::endl
            << "     [" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << "]" << std::endl
            << "     [" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
        out << std::endl << "mat4:" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << "["      << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "]" << std::endl
            << "     [" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "]" << std::endl
            << "     [" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "]" << std::endl
            << "     [" << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "]";

        return out;
    }

    inline std::ostream &operator<< (std::ostream &out, const glm::quat &q) {
        out << "quat:[" << std::fixed << std::setprecision(GLM_OUT_STREAM_PRECISION)
            << q.w << " " << q.x << " "<< q.y << " " << q.z
            << "]";

        return out;
    }

    inline std::ostream & operator << (std::ostream & out, const std::vector<vec3> & vecVector) {
        out << "vector<vec3>[";
        int i = 0;
        for(; i < vecVector.size() - 1; i++) {
            out << vecVector[i] << ", ";
        }
        out << vecVector[i];
        out << "]" << std::endl;

        return out;
    }

    // Needed if gtest EXPECT_PRED2 fails, so it can output detailed error message.
    inline std::ostream & operator << (std::ostream & out, const std::vector<vec2> & vecVector) {
        out << "vector<vec2>[";
        int i = 0;
        for(; i < vecVector.size() - 1; i++) {
            out << vecVector[i] << ", ";
        }
        out << vecVector[i];
        out << "]" << std::endl;

        return out;
    }

}} // end namespace glm::detail

#endif // RIGID3D_GLM_OUT_STREAM_HPP
