SFML-OpenGL
===========

### Dependencies
* Mesa3D
* glm
* glew
* SFML

### Building Libraries (Linux)
#### GTest
* cd build
* <code>premake4 --file="gtest.lua" gmake</code>
* <code>make config=debug</code>
* <code>make config=release</code>

#### GLUtils
* from root folder of project
* <code>premake4 gmake</code>
* <code>make config=debug</code>
* <code>make config=release</code>
