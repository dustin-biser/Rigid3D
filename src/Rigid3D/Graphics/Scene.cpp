#include "Scene.hpp"

namespace Rigid3D {

//----------------------------------------------------------------------------------------
/**
 * Constructor
 * Constructs a Scene using  an 'initiaizizer_list', where each element of the list
 * is a MeshInfo object containing a 'meshName' 'string' and an 'objFile' 'string'.
 * This will form the set of all meshes that can be rendered as part of the Scene.
 *
 * Example:
 * \code
 * Scene scene = { MeshInfo("mesh1", "path/to/mesh/data1.obj"),
 *                 MeshInfo("mesh2", "path/to/mesh/data2.obj")
 *                 ...
 *                 MeshInfo("meshN", "path/to/mesh/dataN.obj")
 *               };
 * \endcode
 *
 * The 'meshName' 'string' identifier for each mesh will be stored internally, and
 * can be used to generate Renderable instances of the mesh using
 * Scene::createRenderable(const string & meshName).
 *
 * @see MeshInfo
 *
 * @param meshList
 */
Scene::Scene(initializer_list<MeshInfo> meshlist) {

//    for(auto key_value : list) {
//        meshName mesh = key_value.first;
//    }

    // 1. Construct a local unordered_map<meshName, Mesh> meshMap.
    // 2. For each pair in init_list construct a Mesh using ObjFileLoader, and
    //    insert the [meshName, Mesh] pair into meshMap.
    // 3. For each [meshName, Mesh] pair in meshMap:
    //      + Copy vertex data from Mesh to Scene object
    //      + Construct a BatchInfo object
    //      + Insert [meshName, BatchInfo] pair into meshBatchMap.

}

//----------------------------------------------------------------------------------------
Scene::~Scene() {

}

} // end namespace Rigid3D
