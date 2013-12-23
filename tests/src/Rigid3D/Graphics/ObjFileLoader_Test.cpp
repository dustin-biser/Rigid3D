#include "ObjFileLoader_Test.hpp"

#include <unordered_map>

// Both the textured and non-textured cubes should have this many indexable vertex entries.
// Manually verified both test_cube.obj and test_cube_textured.obj files.
const int32 ObjFileLoader_Test::expected_num_vertices = 24;

// Manually verified both cube.obj and cube_textured.obj files.
const int32 ObjFileLoader_Test::expected_num_indices = 36;



// Loader_NoTextureCoords static variable definitions.
vector<vec3> Loader_NoTextureCoords::position_set = {
    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, -1.0, -1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 1.0)
};

vector<vec3> Loader_NoTextureCoords::normal_set = {
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 0.0, -1.0),
    vec3(1.0, -0.0, 0.0),
    vec3(0.0, -0.0, 1.0),
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 1.0 ,0.0)
};

vector<vec3> Loader_NoTextureCoords::expected_positions;
vector<vec3> Loader_NoTextureCoords::expected_normals;
vector<uint32> Loader_NoTextureCoords::expected_indices;


// Loader_WithextureCoords static variable definitions.
vector<vec3> Loader_WithTextureCoords::position_set = {
    vec3(1.0, -1.0, -1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, -1.0),
};

vector<vec2> Loader_WithTextureCoords::textureCoord_set = {
    vec2(0.635939, 0.740400),
    vec2(0.391433, 0.740400),
    vec2(0.391433, 0.495894),
    vec2(0.635938, 0.495894),
    vec2(0.635934, 0.006885),
    vec2(0.635936, 0.251388),
    vec2(0.391432, 0.251390),
    vec2(0.391431, 0.006886),
    vec2(0.635939, 0.984906),
    vec2(0.391433, 0.984907),
    vec2(0.146926, 0.984906),
    vec2(0.146927, 0.740400),
    vec2(0.880445, 0.740400),
    vec2(0.880446, 0.984906)
};

vector<vec3> Loader_WithTextureCoords::normal_set = {
vec3(0.0, -1.0, 0.0),
vec3(0.0, 1.0, 0.0),
vec3(1.0, 0.0, 0.0),
vec3(0.0, 0.0, 1.0),
vec3(-1.0, 0.0, 0.0),
vec3(0.0, 0.0, -1.0),
};

vector<vec3> Loader_WithTextureCoords::expected_positions;
vector<vec2> Loader_WithTextureCoords::expected_textureCoords;
vector<vec3> Loader_WithTextureCoords::expected_normals;
vector<uint32> Loader_WithTextureCoords::expected_indices;


namespace {
// For computing hash codes of vec2 types.
class Vec2Hasher {
    public:
        size_t operator () (const vec2 & v) const {
            int result = 17;
            result = 31 * result + v.x;
            result = 31 * result + v.y;

            return result;
        }
    };
}


// Method Definitions
//-----------------------------------------------------------------------------------
template <typename T>
vector<T> ObjFileLoader_Test::buildVector(const T * v, int32 numElements) {
    vector<T> result;

    for(int i = 0; i < numElements; i++) {
        result.push_back(*(v+i));
    }

    return result;
}

//-----------------------------------------------------------------------------------
void Loader_NoTextureCoords::setupExpectedVertexData() {
    vector<int32> pos_indices = { 5, 6, 2, 6, 7, 3, 7, 8, 4, 5, 1, 8, 1, 2, 3, 8, 7, 6, 1,
            5, 2, 2, 6, 3, 3, 7, 4, 8, 1, 4, 4, 1, 3, 5, 8, 6 };

    vector<int32> norm_indices = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 1,
            1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6 };

    // Subtract one from each index so indices start at 0.
    for(int32 &i : pos_indices) {
        i--;
    }
    for(int32 &i : norm_indices) {
        i--;
    }
    assert (pos_indices.size() == norm_indices.size());


    unordered_map<vec2, int32, Vec2Hasher> indexMap;
    int32 indexCounter = 0;

    vec2 indexSet; // (position_index, normal_index)
    bool exists;
    for(int i = 0; i < pos_indices.size(); i++) {
        indexSet = vec2(pos_indices[i], norm_indices[i]);
        exists = indexMap.count(indexSet);
        if (exists) {
            int32 existingIndex = indexMap[indexSet];
            expected_indices.push_back(existingIndex);
        } else {
            // Add new indexSet to indexMap.
            indexMap[indexSet] = indexCounter;

            vec3 position = position_set[indexSet.x];
            expected_positions.push_back(position);

            vec3 normal = normal_set[indexSet.y];
            expected_normals.push_back(normal);

            expected_indices.push_back(indexCounter);

            indexCounter++;
        }
    }

}

//-----------------------------------------------------------------------------------
void Loader_WithTextureCoords::setupExpectedVertexData() {
    vector<uint32> pos_indices = { 1, 2, 3, 1, 3, 4, 5, 8, 7, 5, 7, 6, 1, 5, 6, 1, 6, 2,
            2, 6, 7, 2, 7, 3, 3, 7, 8, 3, 8, 4, 5, 1, 4, 5, 4, 8 };

    vector<uint32> tex_indices = { 1, 2, 3, 1, 3, 4, 5, 6, 7, 5, 7, 8, 1, 9, 10, 1,
            10, 2, 2, 10, 11, 2, 11, 12, 3, 7, 6, 3, 6, 4, 9, 1, 13, 9, 13, 14
    };

    vector<uint32> norm_indices = { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
            3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6 };

    // Subtract one from each index so indices start at 0.
    for(uint32 &i : pos_indices) {
        i--;
    }
    for(uint32 &i : tex_indices) {
        i--;
    }
    for(uint32 &i : norm_indices) {
        i--;
    }
    assert (pos_indices.size() == norm_indices.size());
    assert (pos_indices.size() == tex_indices.size());


    unordered_map<vec3, uint32, Hasher> indexMap;
    uint32 indexCounter = 0;

    vec3 indexSet; // (position_index, normal_index)
    bool exists;
    for(int i = 0; i < pos_indices.size(); i++) {
        indexSet = vec3(pos_indices[i], tex_indices[i], norm_indices[i]);
        exists = indexMap.count(indexSet);
        if (exists) {
            uint32 existingIndex = indexMap[indexSet];
            expected_indices.push_back(existingIndex);
        } else {
            // Add new indexSet to indexMap.
            indexMap[indexSet] = indexCounter;

            vec3 position = position_set[indexSet.x];
            expected_positions.push_back(position);

            vec2 textureCoord = textureCoord_set[indexSet.y];
            expected_textureCoords.push_back(textureCoord);

            vec3 normal = normal_set[indexSet.z];
            expected_normals.push_back(normal);

            expected_indices.push_back(indexCounter);

            indexCounter++;
        }
    }

}

//-----------------------------------------------------------------------------------
namespace glm {
    namespace detail {
        // Needed if gtest EXPECT_PRED2 fails, so it can output detailed error message.
        ostream & operator << (ostream & out, const vector<vec3> & vecVector) {
            out << "vector<vec3>[";
            int i = 0;
            for(; i < vecVector.size() - 1; i++) {
                out << vecVector[i] << ", ";
            }
            out << vecVector[i];
            out << "]" << endl;

            return out;
        }

        // Needed if gtest EXPECT_PRED2 fails, so it can output detailed error message.
        ostream & operator << (ostream & out, const vector<vec2> & vecVector) {
            out << "vector<vec2>[";
            int i = 0;
            for(; i < vecVector.size() - 1; i++) {
                out << vecVector[i] << ", ";
            }
            out << vecVector[i];
            out << "]" << endl;

            return out;
        }
    }
}

// Test that MeshData contains:
// 1. Correct number of positions
// 2. Correct number of normals
// 3. Correct number of textureCoords
// 4. Correct number of indices
// 5. numVertices is correct
// 6. numIndices is correct
// 7. Verify that positions are in the correct order
// 8. Verify that normals are in the correct order
// 9. Verify that textureCoords are in the correct order

//////////////////////////////////////////////////////////////////////////////////////////
// Test Loader_NoTextCoords
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_position_data_access){
    // Attempt to read/write to last location of positions array.
    vec3 last_position = *(meshData.positions + expected_num_vertices - 1);
    *(meshData.positions + expected_num_vertices - 1) = vec3();
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_normal_data_acess){
    // Attempt to read/write to last location of positions array.
    vec3 last_normal = *(meshData.normals + expected_num_vertices - 1);
    *(meshData.normals + expected_num_vertices - 1) = vec3();
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_index_data_access){
    // Attempt to read/write to last location of positions array.
    int32 last_index = *(meshData.indices + expected_num_indices - 1);
    *(meshData.indices + expected_num_indices - 1) = 0;
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_numIndices){
    EXPECT_EQ(expected_num_indices, meshData.numIndices);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_numvertices){
    EXPECT_EQ(expected_num_vertices, meshData.numVertices);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_textureCoords){
    EXPECT_EQ(nullptr, meshData.textureCoords);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_position_data) {
    vector<vec3> positions = buildVector(meshData.positions, meshData.numVertices);
    EXPECT_PRED2(vectors_eq, expected_positions, positions);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_normal_data) {
    vector<vec3> normals = buildVector(meshData.normals, meshData.numVertices);
    EXPECT_PRED2(vectors_eq, expected_normals, normals);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_index_data) {
    vector<uint32> indices = buildVector(meshData.indices, meshData.numIndices);
    EXPECT_EQ(expected_indices.size(), indices.size());
    for(int i = 0; i < expected_indices.size(); i++) {
        EXPECT_EQ(expected_indices[i], indices[i]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test Loader_WithTextCoords
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_position_data_access) {
    // Attempt to read/write to last location of positions array.
    vec3 position = *(meshData.positions + expected_num_vertices - 1);
    *(meshData.positions + expected_num_vertices - 1) = vec3();
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_normal_data_access) {
    // Attempt to read/write to last location of positions array.
    vec3 normal = *(meshData.normals + expected_num_vertices - 1);
    *(meshData.normals + expected_num_vertices - 1) = vec3();
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_textureCoord_data_access) {
    // Attempt to read/write to last location of positions array.
    vec2 textureCoord = *(meshData.textureCoords + expected_num_vertices - 1);
    *(meshData.textureCoords + expected_num_vertices - 1) = vec2();
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_indices_data_access) {
    uint32 index = *(meshData.indices + expected_num_indices - 1);
    *(meshData.indices + expected_num_indices - 1) = 0;
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_numIndices){
    EXPECT_EQ(expected_num_indices, meshData.numIndices);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_number_of_vertices){
    EXPECT_EQ(expected_num_vertices, meshData.numVertices);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_position_data) {
    vector<vec3> positions = buildVector(meshData.positions, meshData.numVertices);
    EXPECT_PRED2(vectors_eq, expected_positions, positions);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_textureCoord_data) {
    vector<vec2> textureCoords = buildVector(meshData.textureCoords, meshData.numVertices);
    EXPECT_EQ(expected_textureCoords.size(), textureCoords.size());
    for(int i = 0; i < expected_textureCoords.size(); i++) {
        EXPECT_PRED2(vec2_eq, expected_textureCoords[i], textureCoords[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_normal_data) {
    vector<vec3> normals = buildVector(meshData.normals, meshData.numVertices);
    EXPECT_PRED2(vectors_eq, expected_normals, normals);
}
