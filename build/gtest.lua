-- Build Static Library for GTest.
project "gtest"
    kind "StaticLib"
    language "C++"
    includedirs {"../ext/gtest/include", "../ext/gtest/"}
    targetdir "../lib"
    files {"../ext/gtest/src/gtest-all.cc","../ext/gtest/src/gtest_main.cc"}

-- Build example test using libgtest.a
project "test1_example"
    kind "ConsoleApp"
    language "C++"
    includedirs {"../ext/gtest/include", "../ext/gtest/"}
    targetdir "../ext/gtest"
    libdirs "../lib"
    links "gtest"
    linkoptions "-lpthread"
    files {"../ext/gtest/sample1.cpp", "../ext/gtest/test1.cpp"}

