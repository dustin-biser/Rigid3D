/**
* Utils.hpp
*
* @author Dustin Biser
*/

#pragma once

#include <cstddef>

namespace utils {

//----------------------------------------------------------------------------------------
/**
* Set each entry of the array pointed to by \c source to \c value.
*/
template<typename T>
void fillArray(void *source, T value, size_t height, size_t width, size_t num_components = 1) {
    for (size_t i(0); i < width * height * num_components; ++i) {
        *(reinterpret_cast<T *>(source) + i) = value;
    }
}
//----------------------------------------------------------------------------------------

} // end namepace utils.
