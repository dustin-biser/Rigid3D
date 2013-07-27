-- Note that all directoires and files are given with respect to where this
-- file is located.

-- Function for creating tests
testLinkLibs = {"GlUtils",
                "GLEW",
                "GL",
                "gtest",
                "sfml-window",
                "sfml-system"}

testLibDirectories = {"../ext/glew-1.10.0/lib",
                      "/usr/local/lib/Mesa-9.1.4",
                      "/usr/lib",
                      "../lib"}

testIncludeDirList = {"../ext/glew-1.10.0/include",
                      "../ext/gtest/include",
                      "../ext/gtest/",
                      "../include",
                      "../include/utils",
                      "../include/utils/GlUtils",
                      "/usr/local/lib/glm-0.9.4.3",
                      "/usr/local/include",
                      "/usr/include"}

function SetupTest(testName, ...)
    project(testName)
    kind "ConsoleApp"
    language "C++"
    location "build"
    objdir "build/obj"
    targetdir "bin"
    includedirs(testIncludeDirList)
    libdirs(testLibDirectories)
    links(testLinkLibs)
    linkoptions "-lpthread"
    buildoptions{"-std=c++0x"}
    files {...}
end

-- Create Unit Tests
SetupTest("Mesh_Test", "src/utils/GlUtils/Mesh_Test.cpp")
SetupTest("ShaderProgram_Test", "src/utils/GlUtils/ShaderProgram_Test.cpp")
