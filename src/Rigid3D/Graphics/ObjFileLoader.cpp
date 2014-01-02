#include "ObjFileLoader.hpp"

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Common/Rigid3DException.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

#include <boost/regex.hpp>

namespace Rigid3D {

using std::endl;
using std::ifstream;
using std::istringstream;
using std::stringstream;
using std::vector;
using boost::regex;
using boost::regex_match;

// Static class definitions.
bool ObjFileLoader::startOfFaceData = true;
vector<vec3> ObjFileLoader::position_set;
vector<vec3> ObjFileLoader::normal_set;
vector<vec2> ObjFileLoader::textureCoord_set;
vector<ivec3> ObjFileLoader::position_indices;
vector<ivec3> ObjFileLoader::normal_indices;
vector<ivec3> ObjFileLoader::textureCoord_indices;

//----------------------------------------------------------------------------------------
MeshData::MeshData()
 : positionSet(nullptr),
   normalSet(nullptr),
   textureCoordSet(nullptr),
   numPositions(0),
   numNormals(0),
   numTextureCoords(0),
   positionIndices(nullptr),
   normalIndices(nullptr),
   textureCoordIndices(nullptr),
   numFaces(0),
   hasTextureCoords(false) {

}

//----------------------------------------------------------------------------------------
/**
 * Reads mesh vertex data from 'objFile' and loads it into the 'meshData' struct.
 *
 * @note The user is responsible for deleting all memory pointed to by
 * 'meshData', by using 'delete []'.
 *
 * @param meshData
 * @param objFile
 */
void ObjFileLoader::load(MeshData & meshData, const string & objFile) {
    ifstream file(objFile, std::ios::in);
    file.exceptions(std::ifstream::badbit);

    if (!file) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFile
                     << " within method Mesh::loadFromObjFile." << endl;
        throw Rigid3DException(errorMessage.str());
    }

    string line;
    uint32 lineCount = 1;

    while (!file.eof()) {
        try {
            getline(file, line);
        } catch(const ifstream::failure & e) {
            file.close();
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw Rigid3DException(errorMessage.str());
        }

        if (line.substr(0,2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            glm::vec3 position; s >> position.x; s >> position.y; s >> position.z;
            position_set.push_back(position);

        } else if (line.substr(0,3) == "vn ") {
            // Normal data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            vec3 normal; s >> normal.x; s >> normal.y; s >> normal.z;
            normal_set.push_back(normal);

        } else if (line.substr(0,3) == "vt ") {
            // Texture coordinate data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            vec2 textureCoord; s >> textureCoord.s; s >> textureCoord.t;
            textureCoord_set.push_back(textureCoord);

        } else if (line.substr(0, 2) == "f ") {
            // Face index data on this line.
            processFaceIndexData(meshData, line, lineCount, objFile);
        }

        lineCount++;
    }

    file.close();

    setMeshData(meshData);

    resetStaticVariables();
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::processFaceIndexData(MeshData & meshData, const string & line, uint32 lineCount,
        const string & objFile) {
    // Index element order = [positionIndex, textureCoordIndex, normalIndex].
    vec3 indexSetA = vec3(0);
    vec3 indexSetB = vec3(0);
    vec3 indexSetC = vec3(0);

    // Regex pattern for face data line containing position, textureCoord, and
    // normal indices.
    regex pos_tex_norm_regex("^f (\\d+/\\d+/\\d+ ){2}\\d+/\\d+/\\d+$");

    // Regex pattern for face data line containing only position and normal
    // indices.
    regex pos_norm_regex("^f (\\d+//\\d+ ){2}\\d+//\\d+$");

    if (regex_match(line, pos_tex_norm_regex)) {
        // Line contains position, texture, and normal indices.
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &indexSetA[0], &indexSetA[1], &indexSetA[2],
                &indexSetB[0], &indexSetB[1], &indexSetB[2],
                &indexSetC[0], &indexSetC[1], &indexSetC[2]);

        if (startOfFaceData) {
            meshData.hasTextureCoords = true;
            startOfFaceData = false;
        }

    } else if (regex_match(line, pos_norm_regex)) {
        // Line contains only position and normal indices.
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &indexSetA[0], &indexSetA[2],
                                                       &indexSetB[0], &indexSetB[2],
                                                       &indexSetC[0], &indexSetC[2]);
        if (startOfFaceData) {
            meshData.hasTextureCoords = false;
            startOfFaceData = false;
        }

    } else {
        stringstream errorMessage;
        errorMessage << "ObjFileLoader Error. Invalid face data at line " << lineCount
                << " within file " << objFile << endl;
        throw Rigid3DException(errorMessage.str());
    }

    // .obj files use indices that start at 1, so subtract 1 so they start at 0.
    indexSetA -= 1;
    indexSetB -= 1;
    indexSetC -= 1;

    ivec3 positionIndices(indexSetA[0], indexSetB[0], indexSetC[0]);
    position_indices.push_back(positionIndices);

    ivec3 normalIndices(indexSetA[2], indexSetB[2], indexSetC[2]);
    normal_indices.push_back(normalIndices);

    if (meshData.hasTextureCoords) {
        ivec3 textureCoordIndices(indexSetA[1], indexSetB[1], indexSetC[1]);
        textureCoord_indices.push_back(textureCoordIndices);
    }
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::setMeshData(MeshData & meshData) {
    // Copy position data.
    meshData.numPositions = position_set.size();
    meshData.positionSet = new vec3[position_set.size()];
    memcpy(meshData.positionSet, position_set.data(), position_set.size() * sizeof(vec3));

    // Copy position indices.
    meshData.positionIndices = new ivec3[position_indices.size()];
    memcpy(meshData.positionIndices, position_indices.data(),
            position_indices.size() * sizeof(ivec3));

    // Copy normal data.
    meshData.numNormals = normal_set.size();
    meshData.normalSet = new vec3[normal_set.size()];
    memcpy(meshData.normalSet, normal_set.data(), normal_set.size() * sizeof(vec3));

    // Copy normal indices.
    meshData.normalIndices = new ivec3[normal_indices.size()];
    memcpy(meshData.normalIndices, normal_indices.data(),
            normal_indices.size() * sizeof(ivec3));

    if (meshData.hasTextureCoords) {
        // Copy textureCoord data.
        meshData.numTextureCoords = textureCoord_set.size();
        meshData.textureCoordSet = new vec2[textureCoord_set.size()];
        memcpy(meshData.textureCoordSet, textureCoord_set.data(),
                textureCoord_set.size() * sizeof(vec2));

        // Copy textureCoord indices.
        meshData.textureCoordIndices = new ivec3[textureCoord_indices.size()];
        memcpy(meshData.textureCoordIndices, textureCoord_indices.data(),
                textureCoord_indices.size() * sizeof(ivec3));
    }

    // Confirm index vectors have the same size.
    assert(position_indices.size() == normal_indices.size());
    if (meshData.hasTextureCoords) {
        assert(position_indices.size() == textureCoord_indices.size());
    }

    // Indices should be a multiple of 3.
    assert(position_indices % 3 == 0);

    meshData.numFaces = position_indices.size() / 3;
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::resetStaticVariables() {
    startOfFaceData = true;
    position_set.clear();
    normal_set.clear();
    textureCoord_set.clear();
    position_indices.clear();
    normal_indices.clear();
    textureCoord_indices.clear();
}

} // end namespace Rigid3D


