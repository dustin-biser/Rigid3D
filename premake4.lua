linkLibs = {"sfml-window",
            "sfml-system",
            "sfml-graphics",
            "GL",
            "GLEW",
            "GlUtils"}

libDirectories = {"/usr/lib",
                  "/usr/local/lib/Mesa-9.1.4",
                  "lib"}

includeDirList = {"ext",
                  "include",
                  "include/examples",
                  "include/utils",
                  "include/utils/GlUtils",
                  "/usr/local/lib/glm-0.9.4.3",
                  "/usr/local/include",
                  "/usr/include"}

solution "SFML-OpenGL-Code"
    configurations { "Debug", "Release" }

    defines {"LOAD_X11"}

    configuration  "Debug"
        defines { "DEBUG" }
        flags {"Symbols", "ExtraWarnings"}

    configuration  "Release"
        defines { "RELEASE", "NDEBUG" }
        flags {"Symbols", "ExtraWarnings"}

    -- Static Library for GlUtils code.
    project "GlUtils"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "lib"
        buildoptions{"-std=c++0x"}
        includedirs(includeDirList)
        files {"src/utils/**.cpp"}

    -- Function for creating demo programs.
    function SetupProject(projName, ...)
        project(projName)
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "bin"
        buildoptions{"-std=c++0x"}
        includedirs(includeDirList)
        libdirs(libDirectories)
        links(linkLibs)
        files { ... }
    end

    -- Function for creating tests
    function SetupTest(testName, ...)
        project(testName)
        kind "ConsoleApp"
        language "C++"
        location "tests/build"
        objdir "tests/build/obj"
        targetdir "tests/bin"
        includedirs(includeDirList)
        includedirs {"ext/gtest/include", "/ext/gtest/"}
        libdirs(libDirectories)
        links(linkLibs)
        links "gtest"
        linkoptions "-lpthread"
        buildoptions{"-std=c++0x"}
        files {...}
    end

-- Build GTest Static Library.
dofile("build/gtest.lua")

-- Create project for each binary
SetupProject("Sfml-Glm-OpenGL-Glew-Example", "src/examples/Sfml-Glm-OpenGL-Glew-Example.cpp")
SetupProject("Triangle_Example", "src/examples/Triangle_Example.cpp", "src/SfmlOpenGLWindow.cpp")
SetupProject("LoadMeshObj-Example", "src/examples/LoadMeshObj-Example.cpp", "src/SfmlOpenGLWindow.cpp")


-- Create Unit Tests
SetupTest("Mesh_Test", "tests/utils/GlUtils/Mesh_Test.cpp")

