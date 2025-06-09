# test/CMakeLists.txt
# -------------------------------------------------------------
# CMake configuration for building and running the test suite.
#
# - Fetches and builds GoogleTest using FetchContent
# - Builds the diff_numerics_tests test binary
# - Registers the test with CTest for automated testing
# -------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(diff_numerics_tests
    ${CMAKE_SOURCE_DIR}/test/test_diff_numerics.cpp
    ${CMAKE_SOURCE_DIR}/src/NumericDiff.cpp
)
target_include_directories(diff_numerics_tests PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/diff_numerics)
target_link_libraries(diff_numerics_tests gtest_main)
target_compile_definitions(diff_numerics_tests PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_test(NAME diff_numerics_tests COMMAND diff_numerics_tests)
