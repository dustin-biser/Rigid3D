/**
 * @brief Mesh_Cube_Test
 *
 * @author Dustin Biser
 */

#include <gtest/gtest.h>

#include <Rigid3D/Graphics/Mesh.hpp>
using Rigid3D::Mesh;

#include <TestUtils.hpp>
using namespace TestUtils::predicates;

#include <glm/glm.hpp>
using glm::vec3;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <vector>
using std::vector;


namespace {  // limit class visibility to this file.

//---------------------------------------------------------------------------------------
    class Mesh_Test : public ::testing::Test {
    protected:
        const static size_t numFaces = 6;
        const static size_t numTrianglesPerFace = 2;
        const static size_t numVerticesPerTriangle = 3;
        const static size_t numNormalsPerTriangle = 3;
        const static size_t numTextureCoordsPerTriangle = 3;

    };

//---------------------------------------------------------------------------------------
    class Mesh_Cube_Test : public Mesh_Test {
    protected:
        static shared_ptr<Mesh> mesh;

        const static size_t expectedTotalVertices;
        const static size_t expectedTotalNormals;

        const static size_t floatsPerVertex = 3;
        const static size_t expectedVertexDataByteSize;

        const static size_t floatsPerNormal = 3;
        const static size_t expectedNormalDataBytesSize;

        // Ran once before all tests.
        static void SetUpTestCase() {
            mesh = make_shared<Mesh>("../data/meshes/cube.obj");
        }

        // Ran once after all tests have finished.
        static void TearDownTestCase() {
        }

        static vector<vec3> buildVector(const float * floatPointer, size_t numElements) {
            vector<vec3> result;
            float x, y, z;

            for(size_t i = 0; i < numElements; i++) {
                x = *floatPointer; floatPointer++;
                y = *floatPointer; floatPointer++;
                z = *floatPointer; floatPointer++;
                result.push_back(vec3(x, y, z));
            }

            return result;
        }

    };

    shared_ptr<Mesh> Mesh_Cube_Test::mesh = nullptr;

    const size_t Mesh_Cube_Test::expectedTotalVertices = numFaces * numTrianglesPerFace * numVerticesPerTriangle;
    const size_t Mesh_Cube_Test::expectedVertexDataByteSize = expectedTotalVertices * floatsPerVertex * sizeof(float);

    const size_t Mesh_Cube_Test::expectedTotalNormals = numFaces * numTrianglesPerFace * numNormalsPerTriangle;
    const size_t Mesh_Cube_Test::expectedNormalDataBytesSize = expectedTotalNormals * floatsPerNormal * sizeof(float);


//---------------------------------------------------------------------------------------
    class Mesh_Textured_Cube_Test : public Mesh_Cube_Test {
    protected:
        static shared_ptr<Mesh> texturedMesh;

        const static size_t expectedTotalTextureCoords;
        const static size_t floatsPerTextureCoord = 2;
        const static size_t expectedTextureCoordDataBytesSize;

        // Ran once before all tests.
        static void SetUpTestCase() {
            texturedMesh = make_shared<Mesh>("../data/meshes/cube_textured.obj");
        }

    };

    shared_ptr<Mesh> Mesh_Textured_Cube_Test::texturedMesh = nullptr;

    const size_t Mesh_Textured_Cube_Test::expectedTotalTextureCoords = numFaces
            * numTrianglesPerFace * numTextureCoordsPerTriangle;

    const size_t Mesh_Textured_Cube_Test::expectedTextureCoordDataBytesSize =
            expectedTotalTextureCoords * floatsPerTextureCoord * sizeof(float);

}

//---------------------------------------------------------------------------------------
/**
 * Test that object file was processed correctly so that Mesh has the right
 * number of vertices.
 */
TEST_F(Mesh_Cube_Test, test_vertex_count){
    EXPECT_EQ(expectedTotalVertices, mesh->getNumVertexPositions());
}

//---------------------------------------------------------------------------------------
/**
 * Test that object file was processed correctly so that Mesh vertex data is the correct
 * size in bytes.
 */
TEST_F(Mesh_Cube_Test, test_vertex_data_bytes){
    EXPECT_EQ(expectedVertexDataByteSize, mesh->getNumVertexPositionBytes());
}

//---------------------------------------------------------------------------------------
/**
 * Test that object file was processed correctly so that Mesh has the right
 * number of normals.
 */
TEST_F(Mesh_Cube_Test, test_normal_count){
    EXPECT_EQ(expectedTotalNormals, mesh->getNumVertexNormals());
}

//---------------------------------------------------------------------------------------
/**
 * Test that object file was processed correctly so that Mesh normal data is the
 * right size in bytes.
 */
TEST_F(Mesh_Cube_Test, test_normal_data_bytes){
    EXPECT_EQ(expectedNormalDataBytesSize, mesh->getNumVertexNormalBytes());
}

/**
 * Test that object file was processed correctly so that normal vectors are in
 * the correct order.
 */
TEST_F(Mesh_Cube_Test, test_order_of_normal_vectors){
    vector<vec3> normals = Mesh_Cube_Test::buildVector(mesh->getVertexNormalDataPtr(), mesh->getNumVertexNormals());

    vec3 n1 = vec3(-1.000000, 0.000000, 0.000000);
    vec3 n2 = vec3(0.000000, 0.000000, -1.000000);
    vec3 n3 = vec3(1.000000, -0.000000, 0.000000);
    vec3 n4 = vec3(0.000000, -0.000000, 1.000000);
    vec3 n5 = vec3(0.000000, -1.000000, 0.000000);
    vec3 n6 = vec3(0.000000, 1.000000, 0.000000);

    vec3 normalSet[] = {n1, n2, n3, n4, n5, n6};
    vector<vec3> expectedNormals;

    int count = 0;
    do {
        for(size_t i = 0, n = 0; n < expectedTotalNormals / 2; i++, n +=3) {
            expectedNormals.push_back(normalSet[i]);
            expectedNormals.push_back(normalSet[i]);
            expectedNormals.push_back(normalSet[i]);
        }
        count++;
    } while(count < 2);

    EXPECT_TRUE(vectors_eq(expectedNormals, normals));
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_vertex_count){
    EXPECT_EQ(expectedTotalVertices, texturedMesh->getNumVertexPositions());
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_vertex_data_bytes){
    EXPECT_EQ(expectedVertexDataByteSize, texturedMesh->getNumVertexPositionBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_number_of_normals){
    EXPECT_EQ(expectedTotalNormals, texturedMesh->getNumVertexNormals());
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_normal_data_bytes){
    EXPECT_EQ(expectedNormalDataBytesSize, texturedMesh->getNumVertexNormalBytes());
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_number_of_textureCoords){
    EXPECT_EQ(expectedTotalTextureCoords, texturedMesh->getNumTextureCoords());
}

//---------------------------------------------------------------------------------------
TEST_F(Mesh_Textured_Cube_Test, test_textureCoord_data_bytes){
    EXPECT_EQ(expectedTextureCoordDataBytesSize, texturedMesh->getNumTextureCoordBytes());
}
