#include "ObjFileLoader.hpp"

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Common/Rigid3DException.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

#include <boost/regex.hpp>

namespace Rigid3D {

using std::endl;
using std::ifstream;
using std::istringstream;
using std::stringstream;
using std::vector;
using std::memcpy;
using boost::regex;
using boost::regex_match;
using boost::smatch;

// Static variable definitions.
FaceDataLayout ObjFileLoader::dataLayout = FaceDataLayout::None;

vector<vec3> ObjFileLoader::positions_ordered;
vector<vec3> ObjFileLoader::position_set;

vector<vec3> ObjFileLoader::normals_ordered;
vector<vec3> ObjFileLoader::normal_set;

vector<vec2> ObjFileLoader::textureCoords_ordered;
vector<vec2> ObjFileLoader::textureCoord_set;

unordered_map<uvec3, uint32, Hasher> ObjFileLoader::indexMap;
vector<uint32> ObjFileLoader::indices;

uint32 ObjFileLoader::indexCounter = 0;


//----------------------------------------------------------------------------------------
/**
 * Reads mesh vertex data from 'objFile' and loads it into the 'meshData' structure.
 *
 * Each position, normal, and textureCoord array of 'meshData' will have its
 * data aligned so that the i-th element from each array corresponds to data for
 * the i-th vertex.  No vertex data triplets (position, textureCoord, normal)
 * will be duplicated within the data arrays of 'meshdata'.
 *
 * @note The user is responsible for deleting all memory pointed to by
 * 'meshData', by using 'delete []'.
 *
 * @param meshData
 * @param objFile
 */
void ObjFileLoader::load(MeshData & meshData, const string & objFile) {
    ifstream in(objFile, std::ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file: " << objFile
                     << " within method Mesh::loadFromObjFile." << endl;
        throw Rigid3DException(errorMessage.str());
    }

    string line;
    uint32 lineCount = 1;

    while (!in.eof()) {
        try {
            getline(in, line);
        } catch(const ifstream::failure & e) {
            in.close();
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw Rigid3DException(errorMessage.str());
        }

        if (line.substr(0,2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(line.substr(2));
            glm::vec3 vertex; s >> vertex.x; s >> vertex.y; s >> vertex.z;
            position_set.push_back(vertex);

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
            processFaceData(line, lineCount, objFile);
        }

        lineCount++;
    }

    in.close();

    // Make sure ordered vectors contain the same number of elements.
    assert(positions_ordered.size() == normals_ordered.size());
    if (dataLayout == FaceDataLayout::PosTextureNorm) {
        assert(positions_ordered.size() == textureCoords_ordered.size());
    }

    copyMeshData(meshData);

    // Free all static vertex data and indices.
    clearData();
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::processFaceData(const string & line, uint32 lineCount,
        const string & objFile) {
    // Element order = [positionIndex, textureCoordIndex, normalIndex].
    uvec3 indexSetA;
    uvec3 indexSetB;
    uvec3 indexSetC;

    // Regex pattern for face data line containing position, textureCoord, and
    // normal indices.
    regex pos_tex_norm_regex("^f (\\d+/\\d+/\\d+ ){2}\\d+/\\d+/\\d+$");

    // Regex pattern for face data line containing only position and normal
    // indices.
    regex pos_norm_regex("^f (\\d+//\\d+ ){2}\\d+//\\d+$");

    if (regex_match(line, pos_tex_norm_regex)) {
        // Line contains position, texture, and normal indices.
        dataLayout = FaceDataLayout::PosTextureNorm;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &indexSetA[0], &indexSetA[1], &indexSetA[2],
                &indexSetB[0], &indexSetB[1], &indexSetB[2],
                &indexSetC[0], &indexSetC[1], &indexSetC[2]);

    } else if (regex_match(line, pos_norm_regex)) {
        // Line contains only position and normal indices.
        dataLayout = FaceDataLayout::PosNorm;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &indexSetA[0], &indexSetA[2],
                                                       &indexSetB[0], &indexSetB[2],
                                                       &indexSetC[0], &indexSetC[2]);
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

    processIndexSet(indexSetA);
    processIndexSet(indexSetB);
    processIndexSet(indexSetC);
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::processIndexSet(const uvec3 indexSet) {
    bool indexSetExists = indexMap.count(indexSet);
    if (indexSetExists) {
        // Append an existing index to end of indices vector.
        uint32 exsitingIndex = indexMap[indexSet];
        indices.push_back(exsitingIndex);
    } else {
        // IndexSet has not been seen yet, so put it in the indexMap with a new index number.
        indexMap[indexSet] = indexCounter;
        indices.push_back(indexCounter);
        indexCounter++;

        // Append ordered vertex data so that all types line up with respect to index.
        if (dataLayout == FaceDataLayout::PosTextureNorm) {
            textureCoords_ordered.push_back(textureCoord_set[indexSet[1]]);
        }

        positions_ordered.push_back(position_set[indexSet[0]]);
        normals_ordered.push_back(normal_set[indexSet[2]]);
    }
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::copyMeshData(MeshData & meshData) {
    size_t numElements = positions_ordered.size();

    // Copy position data.
    meshData.positions = new vec3[numElements];
    memcpy(meshData.positions, positions_ordered.data(), numElements * sizeof(vec3));

    // Copy normal data.
    meshData.normals = new vec3[numElements];
    memcpy(meshData.normals, normals_ordered.data(), numElements * sizeof(vec3));

    // Copy index data.
    meshData.indices = new uint32[indices.size()];
    memcpy(meshData.indices, indices.data(), indices.size() * sizeof(uint32));

    if (dataLayout == FaceDataLayout::PosTextureNorm) {
        // Copy textureCoord data.
        meshData.textureCoords = new vec2[numElements];
        memcpy(meshData.textureCoords, textureCoords_ordered.data(), numElements * sizeof(vec2));
    } else {
        meshData.textureCoords = nullptr;
    }

    meshData.numVertices = numElements;
    meshData.numIndices = indices.size();
}

//----------------------------------------------------------------------------------------
void ObjFileLoader::clearData() {
    dataLayout = FaceDataLayout::None;

    positions_ordered.clear();
    position_set.clear();

    normals_ordered.clear();
    normal_set.clear();

    textureCoords_ordered.clear();
    textureCoord_set.clear();

    indexMap.clear();
    indices.clear();
    indexCounter = 0;
}

} // end namespace Rigid3D


