cmake_minimum_required(VERSION 3.20)
project(sinaps)

set(CMAKE_CXX_STANDARD 17)

# Enable C++ exceptions for Clang-cl
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Xclang -fcxx-exceptions)
endif()

# Build type (static, shared)
option(SINAPS_SHARED "Build SINAPS as a shared library" OFF)
if (SINAPS_SHARED)
    message(STATUS "Building SINAPS as a shared library")
    set(SINAPS_LIBRARY_TYPE SHARED)
else ()
    message(STATUS "Building SINAPS as a static library")
    set(SINAPS_LIBRARY_TYPE STATIC)
endif ()

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(sinaps ${SINAPS_LIBRARY_TYPE} ${SOURCES})

# Add "SINAPS_EXPORTS" to the preprocessor definitions when building the library
if (SINAPS_SHARED)
    target_compile_definitions(sinaps PRIVATE SINAPS_SHARED)
endif ()
target_compile_definitions(sinaps PRIVATE SINAPS_EXPORTS)

# Include directories
target_include_directories(sinaps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)