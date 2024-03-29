ExternalProject_Add(
    binaryen
    URL https://github.com/WebAssembly/binaryen/archive/version_72.zip
    PREFIX binaryen
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DBUILD_STATIC_LIB=OFF
        -DCMAKE_BUILD_TYPE=Release
)

ExternalProject_Get_Property(binaryen source_dir)
ExternalProject_Get_Property(binaryen binary_dir)

file(MAKE_DIRECTORY ${source_dir}/src)

add_library(libbinaryen STATIC IMPORTED)
add_dependencies(libbinaryen binaryen)

set_target_properties(libbinaryen
    PROPERTIES
    IMPORTED_LOCATION ${binary_dir}/lib/libbinaryen.a
    INTERFACE_INCLUDE_DIRECTORIES ${source_dir}/src
)

# target_link_libraries(libbinaryen INTERFACE
# )
