-- Note that all directoires and files are given with respect to where this file is located.

-- Function for creating tests
--testLinkLibs = {"GlUtils", "glfw3", "GLEW", "GL", "gtest" }

testLibDirectories = {"../ext/glew-1.10.0/lib",
                      "/usr/local/lib/Mesa-9.1.4",
                      "/usr/lib",
                      "/usr/local/lib/glfw-3.0.1",
                      "../lib"}

testIncludeDirList = {"include/utils",
                      "../ext/glew-1.10.0/include",
                      "../ext/gtest/include",
                      "../ext/gtest/",
                      "../include",
                      "../include/utils",
                      "../include/utils/GlUtils",
                      "/usr/local/lib/glm-0.9.4.3",
                      "/usr/local/include/SFML-2.1",
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
    links {"GlUtils", "gtest"}
    links(linkLibs)
    linkoptions "-lpthread"
    buildoptions{"-std=c++11"}
    files {...}
end

-- Create Unit Tests
SetupTest("Mesh_Test", "src/utils/GlUtils/Mesh_Test.cpp")
SetupTest("MeshConsolidator_Test", "src/utils/GlUtils/MeshConsolidator_Test.cpp")
SetupTest("ShaderProgram_Test", "src/utils/GlUtils/ShaderProgram_Test.cpp", "../src/GlfwOpenGlWindow.cpp")
SetupTest("GlmOutStream_Test", "src/utils/GlUtils/GlmOutStream_Test.cpp")
