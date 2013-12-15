-- Important: Link order matters for static libraries!  Make sure dependent
-- libraries come before the libraries they depend on.
linkLibs = {"freeimage",
            "Rigid3D",
            "freetype-gl",
            "freetype",
            "glfw3",
            "GLU",
            "GLEW",
            "GL",
            "X11",
            "Xxf86vm",
            "pthread",
            "Xi",
            "Xrandr",
            "boost_regex",
            "stdc++"}

libDirectories = {"ext/glew-1.10.0/lib",
                  "/usr/local/lib/Mesa-9.1.4",
                  "/usr/local/lib/glfw-3.0.1",
                  "/usr/lib",
                  "lib"}

includeDirList = {"ext",
                  "ext/glew-1.10.0/include",
                  "ext/freetype-gl",
                  "include",
                  "include/collision",
                  "include/graphics",
                  "include/math",
                  "examples",
                  "examples/utils",
                  "../glm",
                  "/usr/local/include",
                  "/usr/include",
                  "/usr/include/freetype2/freetype",
                  "/usr/include/freetype2"}

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
    project "Rigid3D"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build/obj"
        targetdir "lib"
        buildoptions{"-std=c++11"}
        includedirs(includeDirList)
        files {"src/**.cpp"}

    -- Function for creating the example programs.
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
--CreateDemo("FreetypeGl_Example", "src/examples/FreetypeGl_Example.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("Glfw-Example", "examples/Glfw-Example.cpp")
CreateDemo("GlfwOpenGlWindowExample", "examples/GlfwOpenGlWindowExample.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("LoadMeshObj_Example", "examples/LoadMeshObj_Example.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("FlatSmoothShading_Example", "examples/FlatSmoothShading_Example.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("MeshNormals", "examples/MeshNormals.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("DepthMapping", "examples/DepthMapping.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("MultipleObjects", "examples/MultipleObjects.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("CameraExample", "examples/CameraExample.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("ShadowMap", "examples/ShadowMap.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
CreateDemo("TexturedCubeDemo", "examples/TexturedCubeDemo.cpp", "examples/utils/GlfwOpenGlWindow.cpp")
