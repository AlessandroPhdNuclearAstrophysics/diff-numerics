// NumericDiffOption.cpp
#include "diff-numerics/NumericDiffOption.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

const std::string NumericDiffOption::usage =
    "Usage: diff-numerics [options] file1 file2\n"
    "Options:\n"
    "  -y, --side-by-side        Print lines side by side\n"
    "  -ys, --suppress-common-lines  Suppress lines within tolerance in side-by-side mode (implies -y)\n"
    "  -t, --tolerance <value>   Tolerance for percentage difference (default: 1E-2)\n"
    "  -T, --threshold <val>     Ignore if both values are under threshold (default: 1E-6)\n"
    "  -c, --comment-string <str> String to start a comment (default: #)\n"
    "  -w, --single-column-width <n> Set column width for side-by-side output (default: 60)\n"
    "  -s, --report-identical-files  Print only if files are equal within tolerance, otherwise print summary\n"
    "  -q, --quiet               Suppress all output if files are equal within tolerance\n"
    "  -d, --color-different-digits  Colorize only the part of the numbers that differ\n"
    "  -C, --columns <list>         Comma-separated list of columns (1-based) to compare\n";

bool NumericDiffOption::parse_columns(const std::string& col_arg, std::set<size_t>& columns_to_compare, const std::string& usage) {
    std::stringstream ss(col_arg);
    std::string col;
    while (std::getline(ss, col, ',')) {
        size_t col_num = std::stoul(col);
        if (col_num < 1) {
            std::cerr << "Error: Column numbers must be at least 1 (got " << col_num << ").\n" << usage;
            return false;
        }
        columns_to_compare.insert(col_num);
    }
    return true;
}

bool NumericDiffOption::parse_args(int argc, char* argv[]) {
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
                return false;
            }
        } else if (arg == "-threshold" || arg == "-T" || arg == "--threshold") {
            if (i + 1 < argc) {
                threshold = std::atof(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return false;
            }
        } else if ((arg == "-comment" || arg == "-c" || arg == "--comment-string")) {
            if (i + 1 < argc) {
                comment_char = argv[++i];
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return false;
            }
        } else if (arg == "-w" || arg == "--single-column-width") {
            if (i + 1 < argc) {
                line_length = std::atoi(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return false;
            }
        } else if (arg == "--only-equal" || arg == "-s" || arg == "--report-identical-files") {
            only_equal = true;
        } else if (arg == "-q" || arg == "--quiet") {
            quiet = true;
        } else if (arg == "-d" || arg == "--color-different-digits") {
            color_diff_digits = true;
        } else if (arg == "-C" || arg == "--columns") {
            if (i + 1 < argc) {
                if (!parse_columns(argv[++i], columns_to_compare, usage)) return false;
            } else {
                std::cerr << "Error: Missing value for " << arg << " option.\n" << usage;
                return false;
            }
        } else if (file1.empty()) {
            file1 = arg;
        } else if (file2.empty()) {
            file2 = arg;
        } else {
            std::cerr << "Unknown or extra argument: " << arg << "\n" << usage;
            return false;
        }
    }
    return true;
}

bool NumericDiffOption::validate_options() const {
    if (file1.empty() || file2.empty()) {
        std::cerr << "Error: Two input files must be specified.\n" << usage;
        return false;
    }
    if (file1 == file2) {
        std::cerr << "Error: The two input files must be different.\n" << usage;
        return false;
    }
    const int min_col_width = 10, max_col_width = 200;
    const double min_tol = 1e-15, max_tol = 1e+3;
    const double min_threshold = 0.0, max_threshold = 1e+3;
    if (line_length < min_col_width || line_length > max_col_width) {
        std::cerr << "Error: Column width (" << line_length << ") must be between " << min_col_width << " and " << max_col_width << ".\n" << usage;
        return false;
    }
    if (tolerance < min_tol || tolerance > max_tol) {
        std::cerr << "Error: Tolerance (" << tolerance << ") must be between " << min_tol << " and " << max_tol << ".\n" << usage;
        return false;
    }
    if (threshold < min_threshold || threshold > max_threshold) {
        std::cerr << "Error: Threshold (" << threshold << ") must be between " << min_threshold << " and " << max_threshold << ".\n" << usage;
        return false;
    }
    return true;
}

// Implement parse and validate as wrappers for parse_args and validate_options
bool NumericDiffOption::parse(int argc, char* argv[]) {
    return parse_args(argc, argv);
}

bool NumericDiffOption::validate() const {
    return validate_options();
}
