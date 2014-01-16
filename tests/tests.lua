-- Note that all directoires and files are given with respect to where this file is located.

-- Function for creating tests
testLinkLibs = {"Rigid3D", "glfw3", "GLEW", "GL", "gtest" }

testLibDirectories = {"../ext/glew-1.10.0/lib",
                      "/usr/local/lib/Mesa-9.1.4",
                      "/usr/lib",
                      "/usr/local/lib/glfw-3.0.1",
                      "../lib"}

testIncludeDirList = { "../ext/glew-1.10.0/include",
                      "../ext/gtest/include",
                      "../ext/gtest/",
                      "../src",
                      "src",
                      "src/Utils",
                      "../../glm",
                      "/usr/local/include",
                      "/usr/include"}

function SetupTest(testName,  ...)
    project(testName)
    kind "ConsoleApp"
    language "C++"
    location "build"
    objdir "build/obj"
    targetdir "bin"
    includedirs(testIncludeDirList)
    libdirs(testLibDirectories)
    links("gtest")
    links(linkLibs)
    linkoptions "-lpthread"
    buildoptions{"-std=c++11"}
    files {...}
end

-- Create Unit Tests
SetupTest("RunAllTests", "src/**")
SetupTest("ShaderProgram_Test", "src/Rigid3D/Graphics/ShaderProgram_Test.cpp", "../src/Rigid3D/Graphics/OpenGlContext.cpp")
SetupTest("GlmOutStream_Test", "src/Rigid3D/Graphics/GlmOutStream_Test.cpp")
SetupTest("Camera_Test", "src/Rigid3D/Graphics/Camera_Test.cpp")
SetupTest("TestUtils_Predicates_Test", "src/Utils/TestUtils_Predicates_Test.cpp")
SetupTest("AABB_Test", "src/Rigid3D/Collision/AABB_Test.cpp")
SetupTest("ObjFileLoader_Test", "src/Rigid3D/Graphics/ObjFileLoader_Test.cpp")
