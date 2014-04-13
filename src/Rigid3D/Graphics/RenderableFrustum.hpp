/**
 * @brief RenderableFrustum
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_RENDERABLEFRUSTUM_HPP_
#define RIGID3D_RENDERABLEFRUSTUM_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/Frustum.hpp>

#include <OpenGL/gltypes.h>

namespace Rigid3D {

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

#endif /* RIGID3D_RENDERABLEFRUSTUM_HPP_ */
