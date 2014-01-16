/**
 * @brief GlmOutStream_Test
 *
 * @author Dustin Biser
 */

#include <gtest/gtest.h>
#include <Rigid3D/Common/GlmOutStream.hpp>
using namespace Rigid3D;

#include <sstream>

namespace {
using std::ostringstream;
using std::streambuf;
using std::stringstream;
using std::cout;
using std::endl;

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
    vec2 v(1,2);
    cout << v;
    EXPECT_EQ("vec2:[1.000000 2.000000]", strCout.str());

}

TEST_F(GlmOutStream_Test, test_vec2_floats) {
    vec2 v(1.0f, 2.0f);
    cout << v;
    EXPECT_EQ("vec2:[1.000000 2.000000]", strCout.str());

}

TEST_F(GlmOutStream_Test, test_vec3_floats) {
    vec3 v(1.0f, 2.0f, 3.0f);
    cout << v;
    EXPECT_EQ("vec3:[1.000000 2.000000 3.000000]", strCout.str());
}

TEST_F(GlmOutStream_Test, test_vec4_floats) {
    vec4 v(1,2,3,4);
    cout << v;
    EXPECT_EQ("vec4:[1.000000 2.000000 3.000000 4.000000]", strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat2_format) {
    mat2 m(1,2,    // first column
           3,4);   // second column
    cout << m;

    stringstream sbuf;
    sbuf << endl << "mat2:[1.000000 3.000000]" << endl
                 << "     [2.000000 4.000000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat3_format) {
    mat3 m(1,2,3,  // first column
           4,5,6,  // second column
           7,8,9); // third column
    cout << m;

    stringstream sbuf;
    sbuf <<  endl << "mat3:[1.000000 4.000000 7.000000]" << endl
                  << "     [2.000000 5.000000 8.000000]" << endl
                  << "     [3.000000 6.000000 9.000000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}

TEST_F(GlmOutStream_Test, test_mat4_format) {
    mat4 m( 1,  2,  3,  4,     // first column
            5,  6,  7,  8,     // second column
            9, 10, 11, 12,     // third column
            13, 14, 15, 16);    // fourth column
    cout << m;

    stringstream sbuf;
    sbuf << endl <<  "mat4:[1.000000 5.000000 9.000000 13.000000]" << endl
                 <<  "     [2.000000 6.000000 10.000000 14.000000]" << endl
                 <<  "     [3.000000 7.000000 11.000000 15.000000]" << endl
                 <<  "     [4.000000 8.000000 12.000000 16.000000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}

TEST_F(GlmOutStream_Test, test_quat_format) {
    quat q(1.0f,  // q_w
           2.0f,  // q_x
           3.0f,  // q_y
           4.0f); // q_z

    cout << q;
    stringstream sbuf;
    sbuf <<  "quat:[1.000000 2.000000 3.000000 4.000000]";

    EXPECT_EQ(sbuf.str(), strCout.str());
}
