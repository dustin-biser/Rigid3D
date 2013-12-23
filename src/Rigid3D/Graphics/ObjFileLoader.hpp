/**
 * @class ObjFileLoader
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_OBJFILELOADER_HPP_
#define RIGID3D_OBJFILELOADER_HPP_

#include <Rigid3D/Common/Settings.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace Rigid3D {
    using std::string;
    using std::unordered_map;
    using std::vector;

    /**
     * A MeshData struct holds all the vertex data needed to render a Mesh.
     */
    struct MeshData {
        vec3 * positions;
        vec3 * normals;
        vec2 * textureCoords;
        uint32 * indices;
        int32 numVertices;
        int32 numIndices;
    };

    enum class FaceDataLayout {
        None,
        PosTextureNorm,
        PosNorm,
    };

    /**
     * For loading mesh data from Wavefront .obj files into MeshData structures.
     */
    class ObjFileLoader {
    public:
        static void load(MeshData & meshData, const string & objFile);

    private:
        ObjFileLoader();

        static void processFaceData(const string & line, uint32 linCount, const string & objFile);

        static void processIndexSet(const uvec3 indexSet);

        static void copyMeshData(MeshData & meshData);

        static void clearData();


        static FaceDataLayout dataLayout;

        static vector<vec3> positions_ordered;
        static vector<vec3> position_set;

        static vector<vec3> normals_ordered;
        static vector<vec3> normal_set;

        static vector<vec2> textureCoords_ordered;
        static vector<vec2> textureCoord_set;

        static unordered_map<uvec3, uint32, Hasher> indexMap;
        static vector<uint32> indices;
        static uint32 indexCounter;
    };

}

#endif /* RIGID3D_OBJFILELOADER_HPP_ */
