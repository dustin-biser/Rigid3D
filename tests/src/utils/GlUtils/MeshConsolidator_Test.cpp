/**
 * @brief MeshConsolidator_Test
 *
 * @author Dustin Biser
 */

#include <MeshConsolidator.hpp>
#include "gtest/gtest.h"
#include "Mesh.hpp"
#include "glm/glm.hpp"
#include <memory>
#include <vector>

using namespace GlUtils;
using std::vector;

namespace {  // limit class visibility to this file.

    class MeshConsolidator_Test : public ::testing::Test {
    protected:
        static const unsigned numberOfCubes = 9;
        static const unsigned sidesPerCube = 6;
        static const unsigned trianglesPerSide = 2;
        static const unsigned verticesPerTriangle = 3;
        static const unsigned floatsPerVertex = 3;
        static const unsigned normalsPerTriangle = 3;
        static const unsigned floatsPerNormal = 3;
    };

    class MeshConsolidator_WithObjFiles_Test : public MeshConsolidator_Test {
    protected:
        static MeshConsolidator meshConsolidator;
        static vector<BatchInfo> batchInfoVec;

        // Ran once before all tests.
        static void SetUpTestCase() {
            meshConsolidator = {"../data/meshes/cube.obj",
                                "../data/meshes/cube_smooth.obj",
                                "../data/meshes/cube.obj",
                                "../data/meshes/cube_smooth.obj",
                                "../data/meshes/cube.obj",
                                "../data/meshes/cube.obj",
                                "../data/meshes/cube_smooth.obj",
                                "../data/meshes/cube_smooth.obj",
                                "../data/meshes/cube.obj"};

            meshConsolidator.getBatchInfo(batchInfoVec);
        }
    };
    // Define static class variables.
    MeshConsolidator MeshConsolidator_WithObjFiles_Test::meshConsolidator;
    vector<BatchInfo> MeshConsolidator_WithObjFiles_Test::batchInfoVec;

    class MeshConsolidator_WithMeshes_Test: public MeshConsolidator_Test {
    protected:
        static MeshConsolidator meshConsolidator;
        static vector<BatchInfo> batchInfoVec;

        // Ran once before all tests.
        static void SetUpTestCase() {
            Mesh m1("../data/meshes/cube.obj");
            Mesh m2("../data/meshes/cube.obj");
            Mesh m3("../data/meshes/cube_smooth.obj");
            Mesh m4("../data/meshes/cube_smooth.obj");
            Mesh m5("../data/meshes/cube.obj");
            Mesh m6("../data/meshes/cube.obj");
            Mesh m7("../data/meshes/cube.obj");
            Mesh m8("../data/meshes/cube_smooth.obj");
            Mesh m9("../data/meshes/cube_smooth.obj");

            meshConsolidator = {&m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9};
            meshConsolidator.getBatchInfo(batchInfoVec);
        }
    };

}

// Define these outside anonymous namespace so their names don't collide with
// MeshConsolidator_WithObjFiles_Test's member variables.
MeshConsolidator MeshConsolidator_WithMeshes_Test::meshConsolidator;
vector<BatchInfo> MeshConsolidator_WithMeshes_Test::batchInfoVec;


//////////////////////////////////////////////////////////////////////////////////////////
// Test with .obj files
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_numVertexBytes){

    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * verticesPerTriangle
                             * floatsPerVertex * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_numNormalBytes){
    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * normalsPerTriangle
                             * floatsPerNormal * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumNormalBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_batchInfoVec) {
    unsigned expected = numberOfCubes;
    EXPECT_EQ(expected, batchInfoVec.size());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_first_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(0);
    EXPECT_EQ(0u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_second_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(1);
    EXPECT_EQ(36u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_third_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(2);
    EXPECT_EQ(72u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_vertexDataPtr) {
    float * vertexDataPtr = const_cast<float *>(meshConsolidator.getVertexDataPtr());

    // Attempt to go to last float in memory block.
    vertexDataPtr += meshConsolidator.getNumVertexBytes() / sizeof(float) ;
    --vertexDataPtr;

    // Attempt to modify last float in memory block.
    *vertexDataPtr += 1.0f;
    *vertexDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_normalDataPtr) {
    float * normalDataPtr = const_cast<float *>(meshConsolidator.getNormalDataPtr());

    // Attempt to go to last float in memory block.
    normalDataPtr += meshConsolidator.getNumNormalBytes() / sizeof(float) ;
    --normalDataPtr;

    // Attempt to modify last float in memory block.
    *normalDataPtr += 1.0f;
    *normalDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Test with Meshes
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_numVertexBytes){

    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * verticesPerTriangle
                             * floatsPerVertex * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_numNormalBytes){
    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * normalsPerTriangle
                             * floatsPerNormal * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumNormalBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_batchInfoVec) {
    unsigned expected = numberOfCubes;
    EXPECT_EQ(expected, batchInfoVec.size());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_first_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(0);
    EXPECT_EQ(0u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_second_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(1);
    EXPECT_EQ(36u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_third_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(2);
    EXPECT_EQ(72u, batchInfo.startIndex);
    EXPECT_EQ(36u, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_vertexDataPtr) {
    float * vertexDataPtr = const_cast<float *>(meshConsolidator.getVertexDataPtr());

    // Attempt to go to last float in memory block.
    vertexDataPtr += meshConsolidator.getNumVertexBytes() / sizeof(float) ;
    --vertexDataPtr;

    // Attempt to modify last float in memory block.
    *vertexDataPtr += 1.0f;
    *vertexDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_normalDataPtr) {
    float * normalDataPtr = const_cast<float *>(meshConsolidator.getNormalDataPtr());

    // Attempt to go to last float in memory block.
    normalDataPtr += meshConsolidator.getNumNormalBytes() / sizeof(float) ;
    --normalDataPtr;

    // Attempt to modify last float in memory block.
    *normalDataPtr += 1.0f;
    *normalDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}
