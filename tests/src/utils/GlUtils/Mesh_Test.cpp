/**
 * \brief Mesh_Test
 *
 * \author Dustin Biser
 */

#include "gtest/gtest.h"
#include "Mesh.hpp"
#include <memory>

using namespace GlUtils;
using namespace std;

namespace {  // limit class visibility to this file.

    class Mesh_Test: public ::testing::Test {
    protected:
        shared_ptr<Mesh> mesh;

        Mesh_Test() {
            mesh = make_shared<Mesh>();
        }

        virtual ~Mesh_Test() {
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

}

TEST_F(Mesh_Test, test_creation_empty_vertex_data){
    EXPECT_EQ(size_t(0), mesh->getVertexDataBytes());
}

TEST_F(Mesh_Test, test_creation_empty_normal_data){
    EXPECT_EQ(size_t(0), mesh->getNormalDataBytes());
}

TEST_F(Mesh_Test, test_creation_empty_index_data){
    EXPECT_EQ(size_t(0), mesh->getIndexDataBytes());
}

/**
 * Test that object file was processed correctly so that Mesh has the right
 * number of vertices, normals, and indices.
 */
TEST_F(Mesh_Test, test_creation_with_obj_file){
    mesh = make_shared<Mesh>("tests/data/cube.obj");

    size_t numberOfVertices = 8;
    size_t numberOfNormals = 8;
    size_t numberOfIndices = 36;

    EXPECT_EQ(numberOfVertices, mesh->getNumVertices());
    EXPECT_EQ(numberOfNormals, mesh->getNumNormals());
    EXPECT_EQ(numberOfIndices, mesh->getNumIndices());

    size_t floatsPerVertex = 4;
    size_t floatsPerNormal = 3;
    size_t floatsPerIndex = 1;

    size_t vertexBytes = numberOfVertices * floatsPerVertex * sizeof(float);
    size_t normalBytes = numberOfNormals * floatsPerNormal * sizeof(float);
    size_t indexBytes = numberOfIndices * floatsPerIndex * sizeof(unsigned short);

    EXPECT_EQ(vertexBytes, mesh->getVertexDataBytes());
    EXPECT_EQ(normalBytes, mesh->getNormalDataBytes());
    EXPECT_EQ(indexBytes, mesh->getIndexDataBytes());
}

