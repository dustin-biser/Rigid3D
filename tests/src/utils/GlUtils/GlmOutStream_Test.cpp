/**
 * @brief GlmOutStream_Test
 *
 * @author Dustin Biser
 */

#include "gtest/gtest.h"
#include "GlmOutStream.hpp"
#include <glm/glm.hpp>
#include <sstream>

using namespace GlUtils;
using namespace std;

namespace {

    class GlmOutStream_Test : public ::testing::Test {
    protected:
        ostringstream strCout;
        streambuf* prevCoutStreamBuf;

        virtual void SetUp() {
            // Redirect cout to the strCout string buffer.
            prevCoutStreamBuf = cout.rdbuf();
            cout.rdbuf(strCout.rdbuf());
        }

        virtual void TearDown() {
            // Restore the previous stream buffer for cout.
            cout.rdbuf(prevCoutStreamBuf);
        }

    };
}

TEST_F(GlmOutStream_Test, test_vec2_ints) {
    glm::vec2 v(1,2);
    cout << v;
    EXPECT_EQ("vec2:[1.0000 2.0000]", strCout.str());

}

TEST_F(GlmOutStream_Test, test_vec2_floats) {
    glm::vec2 v(1.0f, 2.0f);
    cout << v;
    EXPECT_EQ("vec2:[1.0000 2.0000]", strCout.str());

}

TEST_F(GlmOutStream_Test, test_vec3_floats) {
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    cout << v;
    EXPECT_EQ("vec3:[1.0000 2.0000 3.0000]", strCout.str());
}

TEST_F(GlmOutStream_Test, test_vec4_floats) {
    glm::vec4 v(1,2,3,4);
    cout << v;
    EXPECT_EQ("vec4:[1.0000 2.0000 3.0000 4.0000]", strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat2_format) {
    glm::mat2 m(1,2,
                3,4);
    cout << m;

    stringstream sbuf;
    sbuf <<  "mat2:[1.0000 2.0000]" << endl
         <<  "     [3.0000 4.0000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat3_format) {
    glm::mat3 m(1,2,3,
                4,5,6,
                7,8,9);
    cout << m;

    stringstream sbuf;
    sbuf <<  "mat3:[1.0000 2.0000 3.0000]" << endl
         <<  "     [4.0000 5.0000 6.0000]" << endl
         <<  "     [7.0000 8.0000 9.0000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat4_format) {
    glm::mat4 m( 1,  2,  3,  4,
                 5,  6,  7,  8,
                 9, 10, 11, 12,
                13, 14, 15, 16);
    cout << m;

    stringstream sbuf;
    sbuf <<  "mat4:[1.0000 2.0000 3.0000 4.0000]" << endl
         <<  "     [5.0000 6.0000 7.0000 8.0000]" << endl
         <<  "     [9.0000 10.0000 11.0000 12.0000]" << endl
         <<  "     [13.0000 14.0000 15.0000 16.0000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}
