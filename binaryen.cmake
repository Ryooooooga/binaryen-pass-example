if (WIN32)
    set(binaryen_BUILD_STATIC_LIB OFF)
else ()
    set(binaryen_BUILD_STATIC_LIB ON)
endif ()

ExternalProject_Add(
    binaryen
    URL https://github.com/WebAssembly/binaryen/archive/version_72.zip
    PREFIX binaryen
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DBUILD_STATIC_LIB=${binaryen_BUILD_STATIC_LIB}
        -DCMAKE_BUILD_TYPE=Release
)

ExternalProject_Get_Property(binaryen source_dir)
ExternalProject_Get_Property(binaryen binary_dir)

file(MAKE_DIRECTORY ${source_dir}/src)

if (WIN32)
    add_library(libbinaryen SHARED IMPORTED)

    set_target_properties(libbinaryen
        PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/bin/libbinaryen.dll
        IMPORTED_IMPLIB ${binary_dir}/lib/libbinaryen.dll.a
        INTERFACE_INCLUDE_DIRECTORIES ${source_dir}/src
    )
else ()
    add_library(libbinaryen STATIC IMPORTED)

    set_target_properties(libbinaryen
        PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/lib/libbinaryen.a
        INTERFACE_INCLUDE_DIRECTORIES ${source_dir}/src
    )
endif ()

add_dependencies(libbinaryen binaryen)
