// test_diff_numerics.cpp
// -------------------------------------------------------------
// This file contains unit tests for the diff-numerics project.
// It uses GoogleTest to verify the behavior of the NumericDiff class,
// which performs numerical comparison between two data files.
//
// Each test case checks a different configuration or option of the
// NumericDiff tool, using sample data files in the test/ directory.
// -------------------------------------------------------------

#include <gtest/gtest.h>
#include "diff_numerics/NumericDiff.h"
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <array>
#include <memory>

namespace fs = std::filesystem;

// Helper to copy test data to a temp file (not used in current tests)
void copy_file(const std::string& src, const std::string& dst) {
    std::ifstream in(src);
    std::ofstream out(dst);
    out << in.rdbuf();
}

// Helper to get the absolute path to a file relative to the project root
std::string abs_path(const std::string& rel) {
    return (fs::absolute(fs::path("../") / rel)).string();
}

// Helper to get the absolute path to a test data file
#ifndef TEST_DATA_DIR
#error "TEST_DATA_DIR is not defined. Please set it in CMake."
#endif
std::string test_data_path(const std::string& filename) {
    std::string path = std::string(TEST_DATA_DIR) + "/" + filename;
    std::cout << "[TEST] Resolved test data path: " << path << std::endl;
    return path;
}

// Helper to get the project root directory (one level up from test/)
std::string project_root() {
    return fs::absolute(fs::path(TEST_DATA_DIR) / "..").string();
}

// Helper to run NumericDiff and capture its output
// This function constructs a NumericDiff object with the given options,
// runs the comparison, and returns the output as a string.
std::string run_diff(const std::string& file1, const std::string& file2, double tol, double threshold, bool side_by_side, bool suppress_common_lines, bool only_equal, bool quiet) {
    testing::internal::CaptureStdout();
    NumericDiff diff(file1, file2, tol, threshold, side_by_side, "#", 60, suppress_common_lines, only_equal, quiet);
    diff.run();
    return testing::internal::GetCapturedStdout();
}

// Helper to run the diff_numerics binary as a subprocess and capture stderr
std::string run_diff_numerics_cli(const std::string& args) {
    std::array<char, 256> buffer;
    std::string result;
    // Use absolute path to binary and test data
    std::string bin = project_root() + "/build/diff_numerics";
    std::string cmd = bin + " " + args + " 2>&1";
    std::cout << "[TEST] Running CLI: " << cmd << std::endl;
    std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// Test: Default tolerance, files should be equal
TEST(DiffNumerics, IdenticalFilesDefaultTolerance) {
    std::string file1 = test_data_path("delta_3D2_2.dat");
    std::string file2 = test_data_path("delta_3D2.dat");
    std::string output = run_diff(file1, file2, 1E-2, 1E-6, false, false, false, false);
    std::cout << "[TEST] Output: " << output << std::endl;
    EXPECT_TRUE(output.empty() || output.find("equal") != std::string::npos);
}

// Test: Tight tolerance, files should still be equal
TEST(DiffNumerics, IdenticalFilesTightTolerance) {
    std::string file1 = test_data_path("delta_3D2_2.dat");
    std::string file2 = test_data_path("delta_3D2.dat");
    std::string output = run_diff(file1, file2, 1E-10, 1E-12, false, false, false, false);
    EXPECT_TRUE(output.empty() || output.find("equal") != std::string::npos);
}

// Test: Side-by-side output mode
TEST(DiffNumerics, SideBySideOutput) {
    std::string file1 = test_data_path("delta_3D2_2.dat");
    std::string file2 = test_data_path("delta_3D2.dat");
    std::string output = run_diff(file1, file2, 1E-2, 1E-6, true, false, false, false);
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("|"), std::string::npos);
}

// Test: Suppress common lines in output
TEST(DiffNumerics, SuppressCommonLines) {
    std::string file1 = test_data_path("delta_3D2_2.dat");
    std::string file2 = test_data_path("delta_3D2.dat");
    std::string output = run_diff(file1, file2, 1E-2, 1E-6, true, true, false, false);
    // Should only print lines with differences (none for identical files)
    EXPECT_TRUE(output.empty());
}

// Test: Quiet mode (no output expected)
TEST(DiffNumerics, QuietMode) {
    std::string file1 = test_data_path("delta_3D2_2.dat");
    std::string file2 = test_data_path("delta_3D2.dat");
    std::string output = run_diff(file1, file2, 1E-2, 1E-6, false, false, false, true);
    EXPECT_TRUE(output.empty());
}

// Test: Invalid column width
TEST(DiffNumericsCLI, InvalidColumnWidth) {
    std::string out = run_diff_numerics_cli("-w 5 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    std::cout << "[TEST] CLI Output: " << out << std::endl;
    EXPECT_NE(out.find("Error: Column width"), std::string::npos);
    out = run_diff_numerics_cli("-w 500 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    std::cout << "[TEST] CLI Output: " << out << std::endl;
    EXPECT_NE(out.find("Error: Column width"), std::string::npos);
}

// Test: Invalid tolerance
TEST(DiffNumericsCLI, InvalidTolerance) {
    std::string out = run_diff_numerics_cli("-t 1e-20 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    EXPECT_NE(out.find("Error: Tolerance"), std::string::npos);
    out = run_diff_numerics_cli("-t 1e5 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    EXPECT_NE(out.find("Error: Tolerance"), std::string::npos);
}

// Test: Invalid threshold
TEST(DiffNumericsCLI, InvalidThreshold) {
    std::string out = run_diff_numerics_cli("-T -1 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    EXPECT_NE(out.find("Error: Threshold"), std::string::npos);
    out = run_diff_numerics_cli("-T 1e5 " + test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2.dat"));
    EXPECT_NE(out.find("Error: Threshold"), std::string::npos);
}

// Test: Same file error
TEST(DiffNumericsCLI, SameFileError) {
    std::string out = run_diff_numerics_cli(test_data_path("delta_3D2_2.dat") + " " + test_data_path("delta_3D2_2.dat"));
    EXPECT_NE(out.find("Error: The two input files must be different."), std::string::npos);
}

// Test: Missing file error
TEST(DiffNumericsCLI, MissingFileError) {
    std::string out = run_diff_numerics_cli(test_data_path("delta_3D2_2.dat"));
    EXPECT_NE(out.find("Error: Two input files must be specified."), std::string::npos);
}

// Add more tests for different tolerances, thresholds, and options as needed
