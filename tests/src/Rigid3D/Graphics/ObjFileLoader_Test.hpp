#ifndef RIGID3D_OBJFILELOADER_TEST_HPP_
#define RIGID3D_OBJFILELOADER_TEST_HPP_

#include <gtest/gtest.h>

#include <Rigid3D/Graphics/ObjFileLoader.hpp>
using namespace Rigid3D;

#include <Utils/TestUtils.hpp>
using namespace TestUtils;

#include <vector>
#include <iostream>
using namespace std;

class ObjFileLoader_Test : public ::testing::Test {
protected:
    MeshData meshData;
    static const uint32 expected_num_positions;
    static const uint32 expected_num_normals;
    static const uint32 expected_num_faces;

    template <typename T>
    static vector<T> buildVector(const T * v, int32 numElements);

    // Ran before each test.
    virtual void SetUp() {
        // Clear previous MeshData fields.
        meshData = MeshData();
    }

    // Ran after each test.
    virtual void TearDown() {
        // Delete all mesh data that resides on the heap.
        if (meshData.positionSet != nullptr ) {
            delete [] meshData.positionSet;
        }
        meshData.positionSet = nullptr;

        if (meshData.normalSet != nullptr) {
            delete [] meshData.normalSet;
        }
        meshData.normalSet = nullptr;

        if (meshData.textureCoordSet != nullptr) {
            delete [] meshData.textureCoordSet;
        }
        meshData.textureCoordSet = nullptr;

        if (meshData.positionIndices != nullptr) {
            delete [] meshData.positionIndices;
        }
        meshData.positionIndices = nullptr;

        if (meshData.normalIndices != nullptr) {
            delete [] meshData.normalIndices;
        }
        meshData.normalIndices = nullptr;

        if (meshData.textureCoordIndices != nullptr) {
            delete [] meshData.textureCoordIndices;
        }
        meshData.textureCoordIndices = nullptr;
    }

};

class Loader_NoTextureCoords : public ObjFileLoader_Test {
protected:
    static vector<vec3> expected_position_set;
    static vector<vec3> expected_normal_set;

    static vector<uvec3> expected_position_indices;
    static vector<uvec3> expected_normal_indices;

    // Ran before all tests in this test case.
    static void SetUpTestCase();

    // Ran before each test.
    virtual void SetUp() {
        ObjFileLoader_Test::SetUp();
        ObjFileLoader::load(meshData, "../data/meshes/test_cube.obj");
    }

};

class Loader_WithTextureCoords : public ObjFileLoader_Test {
protected:
    static const uint32 expected_num_textureCoords;

    static vector<vec3> expected_position_set;
    static vector<vec3> expected_normal_set;
    static vector<vec2> expected_textureCoord_set;

    static vector<uvec3> expected_position_indices;
    static vector<uvec3> expected_normal_indices;
    static vector<uvec3> expected_textureCoord_indices;

    // Ran before all tests in this test case.
    static void SetUpTestCase();

    // Ran before each test.
    virtual void SetUp() {
        ObjFileLoader_Test::SetUp();
        ObjFileLoader::load(meshData, "../data/meshes/test_cube_textured.obj");
    }
};


#endif /* RIGID3D_OBJFILELOADER_TEST_HPP_ */
