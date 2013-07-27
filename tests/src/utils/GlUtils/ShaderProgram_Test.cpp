/**
 * \brief ShaderProgram_Test
 *
 * \author Dustin Biser
 */

#include <gtest/gtest.h>
#include <ShaderProgram.hpp>
#include <OpenGLContexInitializer.hpp>
#include <memory>

using namespace GlUtils;
using namespace std;

namespace {  // limit class visibility to this file.

    class ShaderProgram_Test: public ::testing::Test {
    protected:
        shared_ptr<ShaderProgram> shaderProgram;
        shared_ptr<OpenGLContextInitializer> glContextInitalizer;

        ShaderProgram_Test() {
            shaderProgram = make_shared<ShaderProgram>();
        }

        // Code here will be called immediately after the constructor (right
        // before each test).
        virtual void SetUp() {
            glContextInitalizer = make_shared<OpenGLContextInitializer>(4,2);
            glContextInitalizer->initContext();
        }

        // Code here will be called immediately after each test (right
        // before the destructor).
        virtual void TearDown() {
        }

    };

}

/**
 * Test loading, compiling, and linking of vertex and fragment shader.
 */
TEST_F(ShaderProgram_Test, test_loadFromFile){
    shaderProgram->loadFromFile("../data/PositionColor.vert", "../data/PositionColor.frag");

    EXPECT_NE((unsigned int)0, shaderProgram->getProgramObject());
}


