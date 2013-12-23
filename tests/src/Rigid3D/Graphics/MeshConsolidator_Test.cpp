/**
 * @brief MeshConsolidator_Test
 *
 * @author Dustin Biser
 */

#include <Rigid3D/Graphics/MeshConsolidator.hpp>
#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include <memory>
using std::shared_ptr;

#include <unordered_map>
using std::unordered_map;

#include <iostream>
using std::ostream;
using std::endl;

using namespace Rigid3D;

// Add ability to print out BatchInfo values.
namespace Rigid3D {
    ostream & operator << (ostream & out, const BatchInfo & batchInfo) {
        out << "BatchInfo[startIndex = " << batchInfo.startIndex << ", numIndices = "
            << batchInfo.numIndices << "]" << endl;

        return out;
    }
}

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
        static unordered_map<const char *, BatchInfo> batchInfoMap;

        // Ran once before all tests.
        static void SetUpTestCase() {
            meshConsolidator = {
                    {"mesh1", "../data/meshes/cube.obj"},
                    {"mesh2", "../data/meshes/cube_smooth.obj"},
                    {"mesh3", "../data/meshes/cube.obj"},
                    {"mesh4", "../data/meshes/cube_smooth.obj"},
                    {"mesh5", "../data/meshes/cube.obj"},
                    {"mesh6", "../data/meshes/cube.obj"},
                    {"mesh7", "../data/meshes/cube_smooth.obj"},
                    {"mesh8", "../data/meshes/cube_smooth.obj"},
                    {"mesh9", "../data/meshes/cube.obj"}
            };

            meshConsolidator.getBatchInfo(batchInfoMap);
        }
    };
    // Define static class variables.
    MeshConsolidator MeshConsolidator_WithObjFiles_Test::meshConsolidator;
    unordered_map<const char *, BatchInfo> MeshConsolidator_WithObjFiles_Test::batchInfoMap;

    class MeshConsolidator_WithMeshes_Test: public MeshConsolidator_Test {
    protected:
        static MeshConsolidator meshConsolidator;
        static unordered_map<const char *, BatchInfo> batchInfoMap;

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

            meshConsolidator = {
                    {"mesh1", &m1},
                    {"mesh2", &m2},
                    {"mesh3", &m3},
                    {"mesh4", &m4},
                    {"mesh5", &m5},
                    {"mesh6", &m6},
                    {"mesh7", &m7},
                    {"mesh8", &m8},
                    {"mesh9", &m9},
            };

            meshConsolidator.getBatchInfo(batchInfoMap);
        }
    };


}

// Define these outside anonymous namespace so their names don't collide with
// MeshConsolidator_WithObjFiles_Test's member variables.
MeshConsolidator MeshConsolidator_WithMeshes_Test::meshConsolidator;
unordered_map<const char *, BatchInfo> MeshConsolidator_WithMeshes_Test::batchInfoMap;

//////////////////////////////////////////////////////////////////////////////////////////
// Test Helper Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool containsKey(const unordered_map<const char *, BatchInfo> batchInfoMap, const char * key) {
    for(auto key_value : batchInfoMap) {
        if(key_value.first == key) {
            return true;
        }
    }

    return false;
}

bool operator == (const BatchInfo & b1, const BatchInfo & b2) {
    if(b1.startIndex == b2.startIndex) {
        if(b1.numIndices == b2.numIndices) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool containsValue(const unordered_map<const char *, BatchInfo> batchInfoMap,
        const BatchInfo & batchInfo) {

    for(auto key_value : batchInfoMap) {
        if(key_value.second == batchInfo) {
            return true;
        }
    }

    return false;
}

::testing::AssertionResult assertContainsValue(const char* m_expr,
                                               const char* n_expr,
                                               const unordered_map<const char *, BatchInfo> & batchInfoMap,
                                               const BatchInfo & batchInfo) {
  if (containsValue(batchInfoMap, batchInfo)) {
    return ::testing::AssertionSuccess();
  }

  return ::testing::AssertionFailure()
      << "The mapped-value " << batchInfo << "is not contained in unordered_map";
}

::testing::AssertionResult assertContainsKey(const char* m_expr,
                                               const char* n_expr,
                                               const unordered_map<const char *, BatchInfo> & batchInfoMap,
                                               const char * key) {
  if (containsKey(batchInfoMap, key)) {
    return ::testing::AssertionSuccess();
  }

  return ::testing::AssertionFailure()
      << "The key-value " << key << "is not contained in unordered_map";
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test with .obj files
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_numVertexBytes){

    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * verticesPerTriangle
                             * floatsPerVertex * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexPositionBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_numNormalBytes){
    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * normalsPerTriangle
                             * floatsPerNormal * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexNormalBytes());
}

//---------------------------------------------------------------------------------------
/*
 * Test that the batchInfoMap contains:
 * - Correct number of entries
 * - Correct key entries, order does not matter.
 * - Correct value entries, order does not matter.
 */
TEST_F(MeshConsolidator_WithObjFiles_Test, test_batchInfoMap) {
    unsigned expected = numberOfCubes;
    EXPECT_EQ(expected, batchInfoMap.size());
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh1");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh2");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh3");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh4");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh5");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh6");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh7");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh8");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh9");

    vector<BatchInfo> batchInfoVec;
    batchInfoVec.reserve(9);
    for(int i = 0; i < 9; i++) {
        BatchInfo b;
        b.startIndex = i*36;
        b.numIndices = 36;
        batchInfoVec.push_back(b);
    }

    for(const BatchInfo & b : batchInfoVec) {
        EXPECT_PRED_FORMAT2(assertContainsValue, batchInfoMap, b);
    }
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_vertexDataPtr) {
    float * vertexDataPtr = const_cast<float *>(meshConsolidator.getVertexPositionDataPtr());

    // Attempt to go to last float in memory block.
    vertexDataPtr += meshConsolidator.getNumVertexPositionBytes() / sizeof(float) ;
    --vertexDataPtr;

    // Attempt to modify last float in memory block.
    *vertexDataPtr += 1.0f;
    *vertexDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithObjFiles_Test, test_normalDataPtr) {
    float * normalDataPtr = const_cast<float *>(meshConsolidator.getVertexNormalDataPtr());

    // Attempt to go to last float in memory block.
    normalDataPtr += meshConsolidator.getNumVertexNormalBytes() / sizeof(float) ;
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

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexPositionBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_numNormalBytes){
    unsigned expectedBytes = numberOfCubes * sidesPerCube * trianglesPerSide * normalsPerTriangle
                             * floatsPerNormal * sizeof(float);

    EXPECT_EQ(expectedBytes, meshConsolidator.getNumVertexNormalBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_batchInfoMap) {
    unsigned expected = numberOfCubes;
    EXPECT_EQ(expected, batchInfoMap.size());
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh1");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh2");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh3");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh4");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh5");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh6");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh7");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh8");
    EXPECT_PRED_FORMAT2(assertContainsKey, batchInfoMap, "mesh9");

    vector<BatchInfo> batchInfoVec;
    batchInfoVec.reserve(9);
    for(int i = 0; i < 9; i++) {
        BatchInfo b;
        b.startIndex = i*36;
        b.numIndices = 36;
        batchInfoVec.push_back(b);
    }

    for(const BatchInfo & b : batchInfoVec) {
        EXPECT_PRED_FORMAT2(assertContainsValue, batchInfoMap, b);
    }
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_vertexDataPtr) {
    float * vertexDataPtr = const_cast<float *>(meshConsolidator.getVertexPositionDataPtr());

    // Attempt to go to last float in memory block.
    vertexDataPtr += meshConsolidator.getNumVertexPositionBytes() / sizeof(float) ;
    --vertexDataPtr;

    // Attempt to modify last float in memory block.
    *vertexDataPtr += 1.0f;
    *vertexDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}

//---------------------------------------------------------------------------------------
TEST_F(MeshConsolidator_WithMeshes_Test, test_normalDataPtr) {
    float * normalDataPtr = const_cast<float *>(meshConsolidator.getVertexNormalDataPtr());

    // Attempt to go to last float in memory block.
    normalDataPtr += meshConsolidator.getNumVertexNormalBytes() / sizeof(float) ;
    --normalDataPtr;

    // Attempt to modify last float in memory block.
    *normalDataPtr += 1.0f;
    *normalDataPtr -= 1.0f;

    ASSERT_TRUE(true);
}
