# CMakeLists.txt
# -------------------------------------------------------------
# Top-level CMake configuration for the diff-numerics project.
#
# - Sets up project metadata and C++ standard
# - Configures build options and output directories
# - Adds the main executable and install rules
# - Integrates GoogleTest for automated testing
# - Installs man page and headers
# -------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

project(diff-numerics
    VERSION 1.0
    DESCRIPTION "A C++ project for numerical diff between two files"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug vs Release configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Include directories for headers
include_directories(include)
include_directories(include/diff-numerics)

# Gather source and header files
file(GLOB SOURCES "src/*.cpp" "src/libs/*.cpp")
file(GLOB HEADERS "include/diff_numerics/*.h")

# Main executable target
add_executable(diff-numerics ${SOURCES})
target_compile_options(diff-numerics PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow
    -Wconversion -Wsign-conversion -Wfloat-equal
    -fPIC
)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install rules for binary, man page, and headers
install(TARGETS diff-numerics DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/diff-numerics.1 DESTINATION share/man/man1)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/diff-numerics DESTINATION include)

# Enable testing and add GoogleTest for tests
include(CTest)
enable_testing()
add_subdirectory(test)