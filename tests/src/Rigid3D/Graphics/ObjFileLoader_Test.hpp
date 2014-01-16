#ifndef RIGID3D_OBJFILELOADER_TEST_HPP_
#define RIGID3D_OBJFILELOADER_TEST_HPP_

#include <gtest/gtest.h>

#include <Rigid3D/Graphics/ObjFileLoader.hpp>
using namespace Rigid3D;

#include <Utils/TestUtils.hpp>
using namespace TestUtils::predicates;

#include <vector>
#include <iostream>
using namespace std;

class ObjFileLoader_Test : public ::testing::Test {
protected:
    MeshData meshData;
    static const int32 expected_num_vertices;
    static const int32 expected_num_indices;

    template <typename T>
    static vector<T> buildVector(const T * v, int32 numElements);

    // Ran before each test.
    virtual void SetUp() {
        meshData.positions = nullptr;
        meshData.normals = nullptr;
        meshData.textureCoords = nullptr;
        meshData.indices = nullptr;
    }

    // Ran after each test.
    virtual void TearDown() {
        // Clean up all mesh data.
        if (meshData.positions != nullptr ) {
            delete [] meshData.positions;
        }
        meshData.positions = nullptr;

        if (meshData.normals != nullptr) {
            delete [] meshData.normals;
        }
        meshData.normals = nullptr;

        if (meshData.textureCoords != nullptr) {
            delete [] meshData.textureCoords;
        }
        meshData.textureCoords = nullptr;

        if (meshData.indices != nullptr) {
            delete [] meshData.indices;
        }
        meshData.indices = nullptr;

        meshData.numIndices = 0;
        meshData.numVertices = 0;
    }

};

class Loader_NoTextureCoords : public ObjFileLoader_Test {
protected:
    static vector<vec3> position_set;
    static vector<vec3> normal_set;

    static vector<vec3> expected_positions;
    static vector<vec3> expected_normals;
    static vector<uint32> expected_indices;

    static void setupExpectedVertexData();

    // Ran once before all tests.
    static void SetUpTestCase() {
        setupExpectedVertexData();
    }

    // Ran before each test.
    virtual void SetUp() {
        ObjFileLoader_Test::SetUp();
        ObjFileLoader::load(meshData, "../data/meshes/test_cube.obj");
    }

};

class Loader_WithTextureCoords : public ObjFileLoader_Test {
protected:
    static vector<vec3> position_set;
    static vector<vec2> textureCoord_set;
    static vector<vec3> normal_set;

    static vector<vec3> expected_positions;
    static vector<vec2> expected_textureCoords;
    static vector<vec3> expected_normals;
    static vector<uint32> expected_indices;

    static void setupExpectedVertexData();

    // Ran once before all tests.
    static void SetUpTestCase() {
        setupExpectedVertexData();
    }

    // Ran before each test.
    virtual void SetUp() {
        ObjFileLoader_Test::SetUp();
        ObjFileLoader::load(meshData, "../data/meshes/test_cube_textured.obj");
    }
};


#endif /* RIGID3D_OBJFILELOADER_TEST_HPP_ */
