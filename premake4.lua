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

    -- Common project settings for each demo.
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

-- Create project for each binary
SetupProject("Sfml-Glm-OpenGL-Glew-Example", "src/examples/Sfml-Glm-OpenGL-Glew-Example.cpp")
SetupProject("Triangle_Example", "src/examples/Triangle_Example.cpp", "src/SfmlOpenGLWindow.cpp")
--SetupProject("LoadMeshObj-Example", "src/examples/LoadMeshObj-Example.cpp", "src/SfmlOpenGLWindow.cpp")

