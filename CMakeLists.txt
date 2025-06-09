cmake_minimum_required(VERSION 3.10)

project(DIFF_NUMERICS
    VERSION 1.0
    DESCRIPTION "A C++ project for numerical diff between two files"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add compiler flags
add_compile_options(
    -Wall -Wextra -Wpedantic -Wshadow 
    -Wconversion -Wsign-conversion -Wfloat-equal
    -fPIC 
)

# Debug vs Release configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


include_directories(headers)
include_directories(headers/libs)

file(GLOB SOURCES "src/*.cpp" "src/libs/*.cpp")
file(GLOB HEADERS "headers/*.h" "headers/libs/*.h")

add_executable(diff_numerics ${SOURCES})

# Set output directory for the executable
set_target_properties(diff_numerics PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

install(TARGETS diff_numerics DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/diff_numerics.1 DESTINATION share/man/man1)