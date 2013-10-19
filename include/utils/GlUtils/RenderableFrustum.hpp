/**
 * @brief RenderableFrustum
 *
 * @author Dustin Biser
 */

#ifndef RENDERABLEFRUSTUM_HPP_
#define RENDERABLEFRUSTUM_HPP_

#include <GL/glew.h>

#include "glm/glm.hpp"
using glm::mat4;
using glm::vec4;

#include "Frustum.hpp"

namespace GlUtils {

    class RenderableFrustum : public Frustum {
    public:
        RenderableFrustum();
        RenderableFrustum(const Frustum & frustum);

        ~RenderableFrustum();

        void render(unsigned int vertexAttribIndex);

        void setModelMatrix(const mat4 & modelMatrix);
        void setViewMatrix(const mat4 & viewMatrix);
        void setColor(const vec4 & rgba);

        RenderableFrustum & operator = (RenderableFrustum && rf);

    private:
        GLuint vao;
        GLuint vbo_vertices;
        mat4 modelMatrix;
        mat4 viewMatrix;
        mat4 ProjectionMatrx;
        vec4 rgba;

        void setupData();
    };
}

#endif /* RENDERABLEFRUSTUM_HPP_ */
