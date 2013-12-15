-- Note that all directoires and files are given with respect to where this file is located.

-- Function for creating tests
testLinkLibs = {"Rigid3D", "glfw3", "GLEW", "GL", "gtest" }

testLibDirectories = {"../ext/glew-1.10.0/lib",
                      "/usr/local/lib/Mesa-9.1.4",
                      "/usr/lib",
                      "/usr/local/lib/glfw-3.0.1",
                      "../lib"}

testIncludeDirList = {"include/utils",
                      "include/collision",
                      "../ext/glew-1.10.0/include",
                      "../ext/gtest/include",
                      "../ext/gtest/",
                      "../include",
                      "../include/graphics",
                      "../include/collision",
                      "../include/math",
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
SetupTest("Mesh_Test", "src/graphics/Mesh_Test.cpp")
SetupTest("MeshConsolidator_Test", "src/graphics/MeshConsolidator_Test.cpp")
SetupTest("ShaderProgram_Test", "src/graphics/ShaderProgram_Test.cpp", "../src/graphics/OpenGlContext.cpp")
SetupTest("GlmOutStream_Test", "src/graphics/GlmOutStream_Test.cpp")
SetupTest("Camera_Test", "src/graphics/Camera_Test.cpp")
SetupTest("TestUtils_Predicates_Test", "src/utils/TestUtils_Predicates_Test.cpp")
SetupTest("AABB_Test", "src/collision/AABB_Test.cpp")
