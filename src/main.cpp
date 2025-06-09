// main.cpp
// -------------------------------------------------------------
// This is the main entry point for the diff-numerics command-line tool.
// It parses command-line arguments, configures options, and runs the
// NumericDiff class to compare two numerical data files.
//
// Usage and options are printed if arguments are missing or invalid.
// -------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "diff-numerics/NumericDiff.h"

int main(int argc, char* argv[]) {
    // Option variables with defaults
    bool side_by_side = false;
    double tolerance = 1E-2;
    double threshold = 1E-6;
    std::string comment_char = "#";
    bool suppress_common_lines = false;
    bool only_equal = false;
    bool quiet = false;
    int line_length = 60;
    bool color_diff_digits = false;
    // Usage message for help
    const std::string usage = "Usage: " + std::string(argv[0]) + " [options] file1 file2\n"
                        "Options:\n"
                        "  -y, --side-by-side        Print lines side by side\n"
                        "  -ys, --suppress-common-lines  Suppress lines within tolerance in side-by-side mode (implies -y)\n"
                        "  -t, --tolerance <value>   Tolerance for percentage difference (default: 1E-2)\n"
                        "  -T, --threshold <val>     Ignore if both values are under threshold (default: 1E-6)\n"
                        "  -c, --comment-string <str> String to start a comment (default: #)\n"
                        "  -w, --single-column-width <n> Set column width for side-by-side output (default: 60)\n"
                        "  -s, --report-identical-files  Print only if files are equal within tolerance, otherwise print summary\n"
                        "  -q, --quiet               Suppress all output if files are equal within tolerance\n"
                        "  -d, --color-different-digits  Colorize only the part of the numbers that differ\n";
    std::string file1, file2;

    // Manual option parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-y" || arg == "--side-by-side") {
            side_by_side = true;
        } else if (arg == "-ys" || arg == "--suppress-common-lines") {
            suppress_common_lines = true;
            side_by_side = true;
        } else if ((arg == "-tol" || arg == "-tolerance" || arg == "-t" || arg == "--tolerance")) {
            if (i + 1 < argc) {
                tolerance = std::atof(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return 1;
            }
        } else if (arg == "-threshold" || arg == "-T" || arg == "--threshold") {
            if (i + 1 < argc) {
                threshold = std::atof(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return 1;
            }
        } else if ((arg == "-comment" || arg == "-c" || arg == "--comment-string")) {
            if (i + 1 < argc) {
                comment_char = argv[++i];
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return 1;
            }
        } else if (arg == "-w" || arg == "--single-column-width") {
            if (i + 1 < argc) {
                line_length = std::atoi(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return 1;
            }
        } else if (arg == "--only-equal" || arg == "-s" || arg == "--report-identical-files") {
            only_equal = true;
        } else if (arg == "-q" || arg == "--quiet") {
            quiet = true;
        } else if (arg == "-d" || arg == "--color-different-digits") {
            color_diff_digits = true;
        } else if (file1.empty()) {
            file1 = arg;
        } else if (file2.empty()) {
            file2 = arg;
        } else {
            std::cerr << "Unknown or extra argument: " << arg << "\n" << usage;
            return 1;
        }
    }

    // Check for required file arguments
    if (file1.empty() || file2.empty()) {
        std::cerr << "Error: Two input files must be specified.\n" << usage;
        return 1;
    }
    if (file1 == file2) {
        std::cerr << "Error: The two input files must be different.\n" << usage;
        return 1;
    }

    // Validate numeric options
    const int min_col_width = 10, max_col_width = 200;
    const double min_tol = 1e-15, max_tol = 1e+3;
    const double min_threshold = 0.0, max_threshold = 1e+3;
    if (line_length < min_col_width || line_length > max_col_width) {
        std::cerr << "Error: Column width (" << line_length << ") must be between " << min_col_width << " and " << max_col_width << ".\n" << usage;
        return 1;
    }
    if (tolerance < min_tol || tolerance > max_tol) {
        std::cerr << "Error: Tolerance (" << tolerance << ") must be between " << min_tol << " and " << max_tol << ".\n" << usage;
        return 1;
    }
    if (threshold < min_threshold || threshold > max_threshold) {
        std::cerr << "Error: Threshold (" << threshold << ") must be between " << min_threshold << " and " << max_threshold << ".\n" << usage;
        return 1;
    }

    // Run the numeric diff with the selected options
    NumericDiff diff(file1, file2, tolerance, threshold, side_by_side, comment_char, line_length, suppress_common_lines, only_equal, quiet, color_diff_digits);
    diff.run();

    return 0;
}