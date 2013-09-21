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

    class MeshConsolidator_Test: public ::testing::Test {
    protected:
        static MeshConsolidator meshConsolidator;
        static vector<BatchInfo> batchInfoVec;

        static const unsigned numberOfCubes = 9;
        static const unsigned sidesPerCube = 6;
        static const unsigned trianglesPerSide = 2;
        static const unsigned verticesPerTriangle = 3;
        static const unsigned floatsPerVertex = 3;
        static const unsigned normalsPerTriangle = 3;
        static const unsigned floatsPerNormal = 3;

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

        // Ran once after all tests have finished.
        static void TearDownTestCase() {
        }

    };

    // Define static class variables.
    MeshConsolidator MeshConsolidator_Test::meshConsolidator;
    vector<BatchInfo> MeshConsolidator_Test::batchInfoVec;

}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_numVertexBytes){

    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * verticesPerTriangle
                             * floatsPerVertex * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_numNormalBytes){
    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * normalsPerTriangle
                             * floatsPerNormal * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumNormalBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_batchInfoVec) {
    unsigned expected = numberOfCubes;
    EXPECT_EQ(expected, batchInfoVec.size());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_first_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(0);
    EXPECT_EQ(0, batchInfo.startIndex);
    EXPECT_EQ(36, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_second_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(1);
    EXPECT_EQ(36, batchInfo.startIndex);
    EXPECT_EQ(36, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_third_batchInfo) {
    BatchInfo batchInfo = batchInfoVec.at(2);
    EXPECT_EQ(72, batchInfo.startIndex);
    EXPECT_EQ(36, batchInfo.numIndices);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_Test, test_vertexDataPtr) {
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
TEST_F(MeshConsolidator_Test, test_normalDataPtr) {
    float * normalDataPtr = const_cast<float *>(meshConsolidator.getNormalDataPtr());

    // Attempt to go to last float in memory block.
    normalDataPtr += meshConsolidator.getNumNormalBytes() / sizeof(float) ;
    --normalDataPtr;

    // Attempt to modify last float in memory block.
    *normalDataPtr += 1.0f;
    *normalDataPtr -= 1.0f;


    ASSERT_TRUE(true);
}
