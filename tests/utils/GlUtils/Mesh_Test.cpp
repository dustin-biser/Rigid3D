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
    EXPECT_EQ(0, mesh->getVertexDataBytes());
}

TEST_F(Mesh_Test, test_creation_empty_normal_data){
    EXPECT_EQ(0, mesh->getNormalDataBytes());
}

TEST_F(Mesh_Test, test_creation_empty_index_data){
    EXPECT_EQ(0, mesh->getIndexDataBytes());
}

TEST_F(Mesh_Test, test_creation_with_obj_file){
    mesh = make_shared<Mesh>("data/cube.obj");

    EXPECT_EQ(8, mesh->getNumVertices());
    EXPECT_EQ(8, mesh->getNumNormals());
    EXPECT_EQ(36, mesh->getNumIndices());
}


