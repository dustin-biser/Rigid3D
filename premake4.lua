-- Important: Link order matters for static libraries!  Make sure dependent
-- libraries come before the libraries they depend on.
linkLibs = {"GlUtils", "MathUtils", "glfw3","GLU", "GLEW", "GL", "X11", "Xxf86vm", "pthread", "Xi", "Xrandr"}

libDirectories = {"ext/glew-1.10.0/lib",
                  "/usr/local/lib/Mesa-9.1.4",
                  "/usr/local/lib/glfw-3.0.1",
                  "/usr/lib",
                  "lib"}

includeDirList = {"ext",
                  "ext/glew-1.10.0/include",
                  "include",
                  "include/examples",
                  "include/utils",
                  "include/utils/GlUtils",
                  "include/utils/MathUtils",
                  "/usr/local/lib/glm-0.9.4.3",
                  "/usr/local/include",
                  "/usr/include"}

solution "Rigid3D"
    configurations { "Debug", "Release" }

    defines {"LOAD_X11", "GLEW_STATIC"}

    configuration  "Debug"
        defines { "DEBUG" }
        flags { "Symbols", "ExtraWarnings" }

    configuration  "Release"
        defines { "RELEASE", "NDEBUG" }
        flags { "Symbols", "ExtraWarnings" }

    -- Build Static Library for GlUtils code.
    project "GlUtils"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "lib"
        buildoptions{"-std=c++11"}
        includedirs(includeDirList)
        files {"src/utils/GlUtils/*.cpp"}

    -- Build Static Library for MathUtils code.
    project "MathUtils"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "lib"
        buildoptions{"-std=c++11"}
        includedirs(includeDirList)
        files {"src/utils/MathUtils/*.cpp"}

    -- Function for creating demo programs.
    function CreateDemo(projName, ...)
        project(projName)
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "bin"
        buildoptions{"-std=c++11"}
        includedirs(includeDirList)
        libdirs(libDirectories)
        links(linkLibs)
        files { ... }
    end

-- Build Tests
dofile("tests/tests.lua")

-- Create a project for each demo
CreateDemo("Triangle_Example", "src/examples/Triangle_Example.cpp", "src/GlfwOpenGLWindow.cpp")
CreateDemo("LoadMeshObj_Example", "src/examples/LoadMeshObj_Example.cpp", "src/GlfwOpenGlWindow.cpp")
CreateDemo("Glfw-Example", "src/examples/Glfw-Example.cpp")
CreateDemo("GlfwOpenGlWindowExample", "src/examples/GlfwOpenGlWindowExample.cpp", "src/GlfwOpenGlWindow.cpp")
