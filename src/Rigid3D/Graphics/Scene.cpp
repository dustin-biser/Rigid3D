#include "Scene.hpp"

namespace Rigid3D {

using std::make_pair;
using std::initializer_list;
using std::pair;

//----------------------------------------------------------------------------------------
/**
 * Constructor
 * Constructs a Scene using  an 'initiaizizer_list', where every two items in
 * the list form a pair consisting of a 'meshName' c-string identifier first,
 * followed by an 'objFile' c-string representing the path to the obj-file
 * containing the mesh.
 *
 * Example:
 * \code
 * Scene scene = {"meshA", "../data/meshes/someMesh.obj",
 *                "meshB", "../data/meshes/someOtherMesh.obj"};
 * \endcode
 *
 * The mesh-name c-string identifier for each mesh will stored internally, and
 * can be used to generate Renderable instances of the mesh using
 * Scene::createRenderable(const char * meshName).
 *
 * @param list
 */
Scene::Scene(initializer_list<pair<meshName, objFile> > list) {

//    for(auto key_value : list) {
//        meshName mesh = key_value.first;
//    }

    // 1. Construct unordered_map<meshName, Mesh> meshMap.
    // 2. For each pair in init_list construct a Mesh using ObjFileLoader, and
    //    insert pair<meshName, Mesh> into meshMap.
    // 3. For each pair in meshMap:
    //      + Copy vertex data from Mesh to Scene object
    //      + Construct a BatchInfo object
}

//----------------------------------------------------------------------------------------
Scene::~Scene() {

}

} // end namespace Rigid3D
