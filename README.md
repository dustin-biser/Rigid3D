Rigid3D
===========

### Dependencies
* freeimage
* glfw3
* glm
* GLEW
* GL

### Building Libraries (Linux)
Building the library currently requires premake4, a free program you can download from here:
http://industriousone.com/premake

#### Rigid3D and Examples
* from root folder of project
* <code>premake4 gmake</code>
* <code>make config=debug</code>
* <code>make config=release</code>

#### GTest Unit Tests
* cd build
* <code>premake4 --file="gtest.lua" gmake</code>
* <code>make config=debug</code>
* <code>make config=release</code>

