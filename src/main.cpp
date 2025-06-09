#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "../headers/libs/NumericDiff.h"

int main(int argc, char* argv[]) {
    bool side_by_side = false;
    double tolerance = 1E-2;
    double threshold = 1E-6;
    std::string comment_char = "#";
    bool suppress_common_lines = false;
    bool only_equal = false;
    bool quiet = false;
    int line_length = 60;
    const std::string usage = "Usage: " + std::string(argv[0]) + " [options] file1 file2\n"
                        "Options:\n"
                        "  -y, --side-by-side        Print lines side by side\n"
                        "  -ys, --suppress-common-lines  Suppress lines within tolerance in side-by-side mode (implies -y)\n"
                        "  -t, --tolerance <value>   Tolerance for percentage difference (default: 1E-2)\n"
                        "  -T, --threshold <val>     Ignore if both values are under threshold (default: 1E-6)\n"
                        "  -c, --comment-string <str> String to start a comment (default: #)\n"
                        "  -w, --single-column-width <n> Set column width for side-by-side output (default: 60)\n"
                        "  -s, --report-identical-files  Print only if files are equal within tolerance, otherwise print summary\n"
                        "  -q, --quiet               Suppress all output if files are equal within tolerance\n";
    std::string file1, file2;

    // Manual option parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-y" || arg == "--side-by-side") {
            side_by_side = true;
        } else if (arg == "-ys" || arg == "--suppress-common-lines") {
            suppress_common_lines = true;
            side_by_side = true;
        } else if ((arg == "-tol" || arg == "-tolerance" || arg == "-t" || arg == "--tolerance") && i + 1 < argc) {
            tolerance = std::atof(argv[++i]);
        } else if (arg == "-threshold" || arg == "-T" || arg == "--threshold") {
            if (i + 1 < argc) {
                threshold = std::atof(argv[++i]);
            }
        } else if ((arg == "-comment" || arg == "-c" || arg == "--comment-string") && i + 1 < argc) {
            comment_char = argv[++i];
        } else if (arg == "-w" || arg == "--single-column-width") {
            if (i + 1 < argc) {
                line_length = std::atoi(argv[++i]);
            }
        } else if (arg == "--only-equal" || arg == "-s" || arg == "--report-identical-files") {
            only_equal = true;
        } else if (arg == "-q" || arg == "--quiet") {
            quiet = true;
        } else if (file1.empty()) {
            file1 = arg;
        } else if (file2.empty()) {
            file2 = arg;
        } else {
            std::cerr << "Unknown or extra argument: " << arg << std::endl;
            return 1;
        }
    }

    if (file1.empty() || file2.empty()) {
        std::cerr << usage;
        return 1;
    }

    NumericDiff diff(file1, file2, tolerance, threshold, side_by_side, comment_char, line_length, suppress_common_lines, only_equal, quiet);
    diff.run();

    return 0;
}