#include <GlfwOpenGlWindow.hpp>
#include <stdio.h>
#include <wchar.h>
#include <freetype-gl.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
using glm::ortho;

#include <glm/gtc/type_ptr.hpp>
using glm::value_ptr;

#include "shader.h"
#include "vertex-buffer.h"

class FreetypeGl_Example : public GlfwOpenGlWindow {
public:
    static shared_ptr<GlfwOpenGlWindow> getInstance() {
        static GlfwOpenGlWindow * instance = new FreetypeGl_Example();
        if (p_instance == nullptr) {
            // Pass ownership of instance to shared_ptr.
            p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
        }
        return p_instance;
    }

private:
    FreetypeGl_Example() { }

    struct vertex_t {
        float x, y, z;    // position
        float s, t;       // texture
        float r, g, b, a; // color
    };

    GLuint shader;
    vertex_buffer_t *buffer;
    mat4   model, view, projection;

    const unsigned int width = 800;
    const unsigned int height = 400;

    void add_text( vertex_buffer_t * buffer, texture_font_t * font,
                   wchar_t * text, vec4 * color, vec2 * pen ) {
        size_t i;
        float r = color->red, g = color->green, b = color->blue, a = color->alpha;
        for( i=0; i<wcslen(text); ++i )
        {
            texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
            if( glyph != NULL )
            {
                int kerning = 0;
                if( i > 0)
                {
                    kerning = texture_glyph_get_kerning( glyph, text[i-1] );
                }
                pen->x += kerning;
                float x0  = (float)( pen->x + glyph->offset_x );
                float y0  = (float)( pen->y + glyph->offset_y );
                float x1  = (float)( x0 + glyph->width );
                float y1  = (float)( y0 - glyph->height );
                float s0 = glyph->s0;
                float t0 = glyph->t0;
                float s1 = glyph->s1;
                float t1 = glyph->t1;
                GLuint indices[6] = {0,1,2, 0,2,3};
                vertex_t vertices[4] = { { x0,y0,0,  s0,t0,  r,g,b,a },
                                         { x0,y1,0,  s0,t1,  r,g,b,a },
                                         { x1,y1,0,  s1,t1,  r,g,b,a },
                                         { x1,y0,0,  s1,t0,  r,g,b,a } };
                vertex_buffer_push_back( buffer, vertices, 4, indices, 6 );
                pen->x += glyph->advance_x;
            }
        }
    }

    virtual void init() {
        size_t i;
        texture_font_t *font = 0;
        texture_atlas_t *atlas = texture_atlas_new( 512, 512, 1 );
        const char * filename = "../ext/freetype-gl/fonts/Vera.ttf";
        const wchar_t * text = L"A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
        buffer = vertex_buffer_new( "vertex:3f,tex_coord:2f,color:4f" );
        vec2 pen = {{5,400}};
        vec4 black = {{0,0,0,1}};
        for( i=7; i < 27; ++i)
        {
            font = texture_font_new( atlas, filename, i );
            pen.x = 5;
            pen.y -= font->height;
            texture_font_load_glyphs( font, text );
            add_text( buffer, font, const_cast<wchar_t *>(text), &black, &pen );
            texture_font_delete( font );
        }
        glBindTexture( GL_TEXTURE_2D, atlas->id );

        shader = shader_load("../ext/freetype-gl/shaders/v3f-t2f-c4f.vert",
                             "../ext/freetype-gl/shaders/v3f-t2f-c4f.frag");
        projection = ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
        model = mat4();
        view = mat4();

        glDisable( GL_DEPTH_TEST );
        glDisable(GL_CULL_FACE);
    }

    virtual void resize(int width, int height) {
        glViewport(0, 0, width, height);
        projection = ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
    }

    virtual void draw() {
        glClearColor( 1, 1, 1, 1 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glUseProgram( shader );
        {
            glUniform1i( glGetUniformLocation( shader, "texture" ),
                         0 );
            glUniformMatrix4fv( glGetUniformLocation( shader, "model" ),
                                1, 0, value_ptr(model));
            glUniformMatrix4fv( glGetUniformLocation( shader, "view" ),
                                1, 0, value_ptr(view));
            glUniformMatrix4fv( glGetUniformLocation( shader, "projection" ),
                                1, 0, value_ptr(projection));
            vertex_buffer_render( buffer, GL_TRIANGLES );
        }
    }

};


int main() {
    shared_ptr<GlfwOpenGlWindow> freetypeDemo = FreetypeGl_Example::getInstance();
    freetypeDemo->create(800, 400, "Freetype-GL Demo");

    return 0;
}
