/**
 * \brief ShaderProgram_Test
 *
 * \author Dustin Biser
 */

#include <gtest/gtest.h>
#include <ShaderProgram.hpp>
#include <ShaderException.hpp>
#include <OpenGlContext.hpp>
#include <memory>

using namespace GlUtils;
using namespace std;

namespace {  // limit class visibility to this file.

    class ShaderProgram_Test: public ::testing::Test {
    protected:

        shared_ptr<ShaderProgram> shaderProgram;
        static shared_ptr<ShaderProgram> goodProgram;
        static shared_ptr<OpenGlContext> glContext;

        ShaderProgram_Test() {
            shaderProgram = make_shared<ShaderProgram>();
        }

        // Code here will be ran once before all tests.
        static void SetUpTestCase() {
            glContext = make_shared<OpenGlContext>(4, 2);
            glContext->init();

            goodProgram = make_shared<ShaderProgram>();
            goodProgram->loadFromFile("../data/shaders/GoodShader.vert", "../data/shaders/GoodShader.frag");
        }

        // Code here will be called immediately after the constructor (right
        // before each test).
        virtual void SetUp() {
        }

        // Code here will be called immediately after each test (right
        // before the destructor).
        virtual void TearDown() {
        }

    };

    // Define static class variables.
    shared_ptr<OpenGlContext> ShaderProgram_Test::glContext;
    shared_ptr<ShaderProgram> ShaderProgram_Test::goodProgram;

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, testPass) {
        ASSERT_TRUE(true);
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief Test loading, compiling, and linking of error free vertex and fragment
     * shaders.  If all goes well, programObject should not equal 0.
     */
    TEST_F(ShaderProgram_Test, test_loadFromFile){
        EXPECT_NE((unsigned int)0, goodProgram->getProgramObject());
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief ShaderException should be thrown on vertex shader compilation error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_vertex_compilation_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/Shader_withSyntaxError.vert",
        "../data/shaders/GoodShader.frag"), ShaderException);
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief ShaderException should be thrown on fragment shader compilation error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_frag_compilation_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/GoodShader.vert",
        "../data/shaders/Shader_withSyntaxError.frag"), ShaderException);
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief ShaderException should be thrown on linker error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_linker_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/GoodShader.vert",
        "../data/shaders/Shader_withLinkerError.frag"), ShaderException);
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief Test getUniformLocation
     */
    TEST_F(ShaderProgram_Test, test_getUniformLocation){
        EXPECT_NE(-1, goodProgram->getUniformLocation("boolUniform"));
    }

    //----------------------------------------------------------------------------------------
    /**
     * @brief Test getAttribLocation
     */
    TEST_F(ShaderProgram_Test, test_getAttribLocation){
        EXPECT_NE(-1, goodProgram->getAttribLocation("position"));
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_bool) {
        goodProgram->setUniform("boolUniform", true);
        GLint uniformLocation = goodProgram->getUniformLocation("boolUniform");
        float values[1];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(true, values[0]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_int) {
        goodProgram->setUniform("intUniform", 99);
        GLint uniformLocation = goodProgram->getUniformLocation("intUniform");
        float values[1];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(99, values[0]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_float) {
        goodProgram->setUniform("floatUniform", 2.0f);
        GLfloat uniformLocation = goodProgram->getUniformLocation("floatUniform");
        float values[1];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(2.0f, values[0]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_uint) {
        goodProgram->setUniform("uintUniform", (unsigned int)22);
        GLfloat uniformLocation = goodProgram->getUniformLocation("uintUniform");
        float values[1];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(22, values[0]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_vec2) {
        vec2 expected = vec2(1.0f, 2.0f);
        goodProgram->setUniform("vec2Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("vec2Uniform");
        float values[2];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected.x, values[0]);
        EXPECT_FLOAT_EQ(expected.y, values[1]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_vec3) {
        vec3 expected = vec3(1.0f, 2.0f, 3.0f);
        goodProgram->setUniform("vec3Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("vec3Uniform");
        float values[3];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected.x, values[0]);
        EXPECT_FLOAT_EQ(expected.y, values[1]);
        EXPECT_FLOAT_EQ(expected.z, values[2]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_vec4) {
        vec4 expected = vec4(1.0f, 2.0f, 3.0f, 4.0f);
        goodProgram->setUniform("vec4Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("vec4Uniform");
        float values[4];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected.x, values[0]);
        EXPECT_FLOAT_EQ(expected.y, values[1]);
        EXPECT_FLOAT_EQ(expected.z, values[2]);
        EXPECT_FLOAT_EQ(expected.w, values[3]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_mat2) {
        mat2 expected = mat2(1.0f, 2.0f,    // first column
                             3.0f, 4.0f);   // second column
        goodProgram->setUniform("mat2Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("mat2Uniform");
        float values[4];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected[0][0], values[0]);
        EXPECT_FLOAT_EQ(expected[0][1], values[1]);
        EXPECT_FLOAT_EQ(expected[1][0], values[2]);
        EXPECT_FLOAT_EQ(expected[1][1], values[3]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_mat3) {
        mat3 expected = mat3(1.0f, 2.0f, 3.0f,   // first column
                             4.0f, 5.0f, 6.0f,   // second column
                             7.0f, 8.0f, 9.0f);  // third column
        goodProgram->setUniform("mat3Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("mat3Uniform");
        float values[9];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected[0][0], values[0]);
        EXPECT_FLOAT_EQ(expected[0][1], values[1]);
        EXPECT_FLOAT_EQ(expected[0][2], values[2]);
        EXPECT_FLOAT_EQ(expected[1][0], values[3]);
        EXPECT_FLOAT_EQ(expected[1][1], values[4]);
        EXPECT_FLOAT_EQ(expected[1][2], values[5]);
        EXPECT_FLOAT_EQ(expected[2][0], values[6]);
        EXPECT_FLOAT_EQ(expected[2][1], values[7]);
        EXPECT_FLOAT_EQ(expected[2][2], values[8]);
    }

    //----------------------------------------------------------------------------------------
    TEST_F(ShaderProgram_Test, test_setUniform_mat4) {
        mat4 expected = mat4(1.0f, 2.0f, 3.0f, 4.0f,      // first column
                             5.0f, 6.0f, 7.0f, 8.0f,      // second column
                             9.0f, 10.0f, 11.0f, 12.0f,   // third column
                             13.0f, 14.0f, 15.0f, 16.0f); // fourth column
        goodProgram->setUniform("mat4Uniform", expected);
        GLfloat uniformLocation = goodProgram->getUniformLocation("mat4Uniform");
        float values[16];
        glGetUniformfv(goodProgram->getProgramObject(), uniformLocation, values);

        EXPECT_FLOAT_EQ(expected[0][0], values[0]);
        EXPECT_FLOAT_EQ(expected[0][1], values[1]);
        EXPECT_FLOAT_EQ(expected[0][2], values[2]);
        EXPECT_FLOAT_EQ(expected[0][3], values[3]);

        EXPECT_FLOAT_EQ(expected[1][0], values[4]);
        EXPECT_FLOAT_EQ(expected[1][1], values[5]);
        EXPECT_FLOAT_EQ(expected[1][2], values[6]);
        EXPECT_FLOAT_EQ(expected[1][3], values[7]);

        EXPECT_FLOAT_EQ(expected[2][0], values[8]);
        EXPECT_FLOAT_EQ(expected[2][1], values[9]);
        EXPECT_FLOAT_EQ(expected[2][2], values[10]);
        EXPECT_FLOAT_EQ(expected[2][3], values[11]);

        EXPECT_FLOAT_EQ(expected[3][0], values[12]);
        EXPECT_FLOAT_EQ(expected[3][1], values[13]);
        EXPECT_FLOAT_EQ(expected[3][2], values[14]);
        EXPECT_FLOAT_EQ(expected[3][3], values[15]);
    }

} // end namespace
