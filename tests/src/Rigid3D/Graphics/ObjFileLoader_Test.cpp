#include "ObjFileLoader_Test.hpp"

// These values hold for both the textured and non-textured test cases.
const uint32 ObjFileLoader_Test::expected_num_positions = 8;
const uint32 ObjFileLoader_Test::expected_num_normals = 6;
const uint32 ObjFileLoader_Test::expected_num_faces = 12;

//---------------------------------------------------------------------------------------
// Loader_NoTextureCoords static variable definitions.
vector<vec3> Loader_NoTextureCoords::expected_position_set = {
    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, -1.0, -1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 1.0)
};

vector<vec3> Loader_NoTextureCoords::expected_normal_set = {
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 0.0, -1.0),
    vec3(1.0, -0.0, 0.0),
    vec3(0.0, -0.0, 1.0),
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 1.0, 0.0)
};

vector<uvec3> Loader_NoTextureCoords::expected_position_indices = {
    uvec3(5, 6, 2),
    uvec3(6, 7, 3),
    uvec3(7, 8, 4),
    uvec3(5, 1, 8),
    uvec3(1, 2, 3),
    uvec3(8, 7, 6),
    uvec3(1, 5, 2),
    uvec3(2, 6, 3),
    uvec3(3, 7, 4),
    uvec3(8, 1, 4),
    uvec3(4, 1, 3),
    uvec3(5, 8, 6)
};

vector<uvec3> Loader_NoTextureCoords::expected_normal_indices = {
    uvec3(1, 1, 1),
    uvec3(2, 2, 2),
    uvec3(3, 3, 3),
    uvec3(4, 4, 4),
    uvec3(5, 5, 5),
    uvec3(6, 6, 6),
    uvec3(1, 1, 1),
    uvec3(2, 2, 2),
    uvec3(3, 3, 3),
    uvec3(4, 4, 4),
    uvec3(5, 5, 5),
    uvec3(6, 6, 6)
};


//---------------------------------------------------------------------------------------
// Loader_WithextureCoords static variable definitions.

const uint32 Loader_WithTextureCoords::expected_num_textureCoords = 14;

vector<vec3> Loader_WithTextureCoords::expected_position_set = {
    vec3(1.0, -1.0, -1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, -1.0),
};

vector<vec2> Loader_WithTextureCoords::expected_textureCoord_set = {
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

vector<vec3> Loader_WithTextureCoords::expected_normal_set = {
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 0.0, 1.0),
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 0.0, -1.0),
};

vector<uvec3> Loader_WithTextureCoords::expected_position_indices = {
    uvec3(1, 2, 3),
    uvec3(1, 3, 4),
    uvec3(5, 8, 7),
    uvec3(5, 7, 6),
    uvec3(1, 5, 6),
    uvec3(1, 6, 2),
    uvec3(2, 6, 7),
    uvec3(2, 7, 3),
    uvec3(3, 7, 8),
    uvec3(3, 8, 4),
    uvec3(5, 1, 4),
    uvec3(5, 4, 8)
};

vector<uvec3> Loader_WithTextureCoords::expected_normal_indices = {
    uvec3(1, 1, 1),
    uvec3(1, 1, 1),
    uvec3(2, 2, 2),
    uvec3(2, 2, 2),
    uvec3(3, 3, 3),
    uvec3(3, 3, 3),
    uvec3(4, 4, 4),
    uvec3(4, 4, 4),
    uvec3(5, 5, 5),
    uvec3(5, 5, 5),
    uvec3(6, 6, 6),
    uvec3(6, 6, 6),
};


vector<uvec3> Loader_WithTextureCoords::expected_textureCoord_indices = {
    uvec3(1, 2, 3),
    uvec3(1, 3, 4),
    uvec3(5, 6, 7),
    uvec3(5, 7, 8),
    uvec3(1, 9, 10),
    uvec3(1, 10, 2),
    uvec3(2, 10, 11),
    uvec3(2, 11, 12),
    uvec3(3, 7, 6),
    uvec3(3, 6, 4),
    uvec3(9, 1, 13),
    uvec3(9, 13, 14)
};


//-----------------------------------------------------------------------------------
void Loader_NoTextureCoords::SetUpTestCase() {
    //-- Decrement all indices by 1.
    for(uvec3 & uvec : expected_position_indices) {
        uvec -= uvec3(1, 1, 1);
    }

    for(uvec3 & uvec : expected_normal_indices) {
        uvec -= uvec3(1, 1, 1);
    }
}

//-----------------------------------------------------------------------------------
void Loader_WithTextureCoords::SetUpTestCase() {
    //-- Decrement all indices by 1.
    for(uvec3 & uvec : expected_position_indices) {
        uvec -= uvec3(1, 1, 1);
    }

    for(uvec3 & uvec : expected_normal_indices) {
        uvec -= uvec3(1, 1, 1);
    }

    for(uvec3 & uvec : expected_textureCoord_indices) {
        uvec -= uvec3(1, 1, 1);
    }
}

// Test that MeshData contains:
// 1. Correct number of positions
// 2. Correct number of normals
// 3. Correct number of textureCoords
// 4. Correct number of various indices
// 7. Verify that positions are in the correct order
// 8. Verify that normals are in the correct order
// 9. Verify that textureCoords are in the correct order

//////////////////////////////////////////////////////////////////////////////////////////
// Test Loader_NoTextCoords
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_field_hasTextureCoords) {
    EXPECT_FALSE(meshData.hasTextureCoords);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_position_set) {
    for(int i = 0; i < expected_position_set.size(); ++i) {
        EXPECT_PRED2(vec3_eq, expected_position_set[i], meshData.positionSet[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_normal_set) {
    for(int i = 0; i < expected_normal_set.size(); ++i) {
        EXPECT_PRED2(vec3_eq, expected_normal_set[i], meshData.normalSet[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_num_positions) {
    EXPECT_EQ(expected_num_positions, meshData.numPositions);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_num_normals) {
    EXPECT_EQ(expected_num_normals, meshData.numNormals);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_num_textureCoords) {
    EXPECT_EQ(0, meshData.numTextureCoords);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_num_faces) {
    EXPECT_EQ(expected_num_faces, meshData.numFaces);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_textureCoords_indices_is_null) {
    EXPECT_EQ(nullptr, meshData.textureCoordIndices);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_position_indices) {
    for(int i = 0; i < expected_position_indices.size(); ++i) {
        EXPECT_PRED2(uvec3_eq, expected_position_indices[i], meshData.positionIndices[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_NoTextureCoords, test_normal_indices) {
    for(int i = 0; i < expected_normal_indices.size(); ++i) {
        EXPECT_PRED2(uvec3_eq, expected_normal_indices[i], meshData.normalIndices[i]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Test Loader_WithTextCoords
//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_field_hasTextureCoords) {
    EXPECT_TRUE(meshData.hasTextureCoords);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_position_set) {
    for(int i = 0; i < expected_position_set.size(); ++i) {
        EXPECT_PRED2(vec3_eq, expected_position_set[i], meshData.positionSet[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_normal_set) {
    for(int i = 0; i < expected_normal_set.size(); ++i) {
        EXPECT_PRED2(vec3_eq, expected_normal_set[i], meshData.normalSet[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_textureCoord_set) {
    for(int i = 0; i < expected_textureCoord_set.size(); ++i) {
        EXPECT_PRED2(vec2_eq, expected_textureCoord_set[i], meshData.textureCoordSet[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_num_positions) {
    EXPECT_EQ(expected_num_positions, meshData.numPositions);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_num_normals) {
    EXPECT_EQ(expected_num_normals, meshData.numNormals);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_num_textureCoords) {
    EXPECT_EQ(expected_num_textureCoords, meshData.numTextureCoords);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_num_faces) {
    EXPECT_EQ(expected_num_faces, meshData.numFaces);
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_textureCoords_indices) {
    for(int i = 0; i < expected_textureCoord_indices.size(); ++i) {
        EXPECT_PRED2(uvec3_eq, expected_textureCoord_indices[i],
                meshData.textureCoordIndices[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_position_indices) {
    for(int i = 0; i < expected_position_indices.size(); ++i) {
        EXPECT_PRED2(uvec3_eq, expected_position_indices[i], meshData.positionIndices[i]);
    }
}

//----------------------------------------------------------------------------------------
TEST_F(Loader_WithTextureCoords, test_normal_indices) {
    for(int i = 0; i < expected_normal_indices.size(); ++i) {
        EXPECT_PRED2(uvec3_eq, expected_normal_indices[i], meshData.normalIndices[i]);
    }
}
