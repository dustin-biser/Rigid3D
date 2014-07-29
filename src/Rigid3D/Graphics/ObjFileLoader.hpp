/**
* @brief ObjFileLoader
*
* @author Dustin Biser
*/

#ifndef RIGID3D_OBJ_FILE_LOADER_HPP_
#define RIGID3D_OBJ_FILE_LOADER_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <vector>

namespace Rigid3D {

class ObjFileLoader {
public:
    static void decode(const char * objFilePath,
                       std::vector<vec3> & positions,
                       std::vector<vec3> & normals,
                       std::vector<vec2> & uvCoords);

    static void decode(const char * objFilePath,
                       std::vector<vec3> & positions,
                       std::vector<vec3> & normals);

};

}

#endif // RIGID3D_OBJ_FILE_LOADER_HPP_
