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
        static shared_ptr<OpenGlContext> glContext;

        ShaderProgram_Test() {
            shaderProgram = make_shared<ShaderProgram>();
        }

        // Code here will be ran once before all tests.
        static void SetUpTestCase() {
            glContext = make_shared<OpenGlContext>(4, 2);
            glContext->init();
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

    shared_ptr<OpenGlContext> ShaderProgram_Test::glContext = nullptr;

    TEST_F(ShaderProgram_Test, testPass) {
        ASSERT_TRUE(true);
    }

    /**
     * @brief Test loading, compiling, and linking of error free vertex and fragment
     * shaders.  If all goes well, programObject should not equal 0.
     */
    TEST_F(ShaderProgram_Test, test_loadFromFile){
        shaderProgram->loadFromFile("../data/shaders/GoodShader.vert", "../data/shaders/GoodShader.frag");

        EXPECT_NE((unsigned int)0, shaderProgram->getProgramObject());
    }

    /**
     * @brief ShaderException should be thrown on vertex shader compilation error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_vertex_compilation_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/Shader_withSyntaxError.vert",
        "../data/shaders/GoodShader.frag"), ShaderException);
    }

    /**
     * @brief ShaderException should be thrown on fragment shader compilation error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_frag_compilation_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/GoodShader.vert",
        "../data/shaders/Shader_withSyntaxError.frag"), ShaderException);
    }

    /**
     * @brief ShaderException should be thrown on linker error.
     */
    TEST_F(ShaderProgram_Test, test_throws_on_linker_error){
        ASSERT_THROW(shaderProgram->loadFromFile("../data/shaders/GoodShader.vert",
        "../data/shaders/Shader_withLinkerError.frag"), ShaderException);
    }

    /**
     * @brief Test getUniformLocation
     */
    TEST_F(ShaderProgram_Test, test_getUniformLocation){
        shaderProgram->loadFromFile("../data/shaders/GoodShader.vert", "../data/shaders/GoodShader.frag");

        EXPECT_NE(-1, shaderProgram->getUniformLocation("colorUniform"));
    }

    /**
     * @brief Test getAttribLocation
     */
    TEST_F(ShaderProgram_Test, test_getAttribLocation){
        shaderProgram->loadFromFile("../data/shaders/GoodShader.vert", "../data/shaders/GoodShader.frag");

        EXPECT_NE(-1, shaderProgram->getAttribLocation("position"));
    }

} // end namespace
