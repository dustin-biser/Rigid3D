/**
 * @class ObjFileLoader
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_OBJFILELOADER_HPP_
#define RIGID3D_OBJFILELOADER_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/Material.hpp>

#include <string>
#include <vector>


namespace Rigid3D {
    using std::string;
    using std::vector;

    /**
     * A MeshData struct holds all the vertex data needed to render a Mesh.
     *
     * The ith index triplet of 'positionIndices', 'normalIndices', and
     * 'textureCoordIndices' each correspond to the 3 indices that from the ith
     * face of the mesh. These indices refer to the elements within
     * 'positionSet', 'normalSet', and 'textureCoordSets', respectively.
     */
    struct MeshData {
        vec3 * positionSet;
        vec3 * normalSet;
        vec2 * textureCoordSet;

        int32 numPositions;
        int32 numNormals;
        int32 numTextureCoords;

        ivec3 * positionIndices;
        ivec3 * normalIndices;
        ivec3 * textureCoordIndices;

        int32 numFaces;

        bool hasTextureCoords;

        MeshData();
    };

    /**
     * For loading mesh data from Wavefront .obj files into MeshData structures.
     */
    class ObjFileLoader {
    public:
        static void load(MeshData & meshData, const string & objFile);

    private:
        static bool startOfFaceData;
        static vector<vec3> position_set;
        static vector<vec3> normal_set;
        static vector<vec2> textureCoord_set;
        static vector<ivec3> position_indices;
        static vector<ivec3> normal_indices;
        static vector<ivec3> textureCoord_indices;

        ObjFileLoader();

        static void processFaceIndexData(MeshData & meshData, const string & line,
                uint32 linCount, const string & objFile);

        static void setMeshData(MeshData & meshData);

        static void resetStaticVariables();

    };

}

#endif /* RIGID3D_OBJFILELOADER_HPP_ */
