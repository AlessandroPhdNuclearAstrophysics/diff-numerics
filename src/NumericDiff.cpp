// NumericDiff.cpp
// -------------------------------------------------------------
// This file implements the NumericDiff class, which compares two numerical
// data files line by line, with configurable tolerance, threshold, and output
// options. It provides the core logic for the diff-numerics tool.
//
// Key features:
// - Compares files line by line, token by token
// - Supports side-by-side and diff-style output
// - Highlights differences above a given tolerance
// - Handles comments, column widths, and summary statistics
// -------------------------------------------------------------

#include "../include/diff_numerics/NumericDiff.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iomanip> // For std::setw

// Constructor: initialize all options and file paths
NumericDiff::NumericDiff(const std::string& file1, const std::string& file2, double tol, double threshold, bool side_by_side, const std::string& comment_char, int line_length, bool suppress_common_lines, bool only_equal, bool quiet)
    : file1_(file1), file2_(file2), tol_(tol), threshold_(threshold), side_by_side_(side_by_side), comment_char_(comment_char), line_length_(line_length), suppress_common_lines_(suppress_common_lines), only_equal_(only_equal), quiet_(quiet), diff_lines_(0), max_percentage_error_(0.0) {}

// Main entry: run the comparison and print results
void NumericDiff::run() {
    diff_lines_ = 0;
    max_percentage_error_ = 0.0;
    std::ifstream fin1(file1_), fin2(file2_);
    if (!fin1.is_open() || !fin2.is_open()) {
        std::cerr << "Error opening files.\n";
        return;
    }
    std::string line1, line2;
    size_t total_lines = 0;
    while (std::getline(fin1, line1) && std::getline(fin2, line2)) {
        if ((!comment_char_.empty() && line1.find(comment_char_) == 0) ||
            (!comment_char_.empty() && line2.find(comment_char_) == 0)) {
            continue;
        }
        ++total_lines;
        compareLine(line1, line2);
    }
    while (std::getline(fin1, line1)) {
        if (!comment_char_.empty() && line1.find(comment_char_) == 0) continue;
        ++total_lines;
        compareLine(line1, "");
    }
    while (std::getline(fin2, line2)) {
        if (!comment_char_.empty() && line2.find(comment_char_) == 0) continue;
        ++total_lines;
        compareLine("", line2);
    }

    if (quiet_) {
        // Print nothing if files are equal, otherwise print as normal (with all options except quiet)
        if (diff_lines_ == 0) {
            return;
        } else {
            // Print summary as in only_equal_ mode
            std::cout << "Comparing " << file1_ << " and " << file2_ << "\n";
            std::cout << "Tolerance: " << tol_ << ", Threshold: " << threshold_ << "\n";
            std::cout << "Files DIFFER: " << diff_lines_ << " lines differ, max percentage error: " << max_percentage_error_ << "%\n";
        }
        return;
    }

    if (only_equal_) {
        std::cout << "Comparing " << file1_ << " and " << file2_ << "\n";
        std::cout << "Tolerance: " << tol_ << ", Threshold: " << threshold_ << "\n";
        if (diff_lines_ == 0) {
            std::cout << "Files are EQUAL within tolerance.\n";
        } else {
            std::cout << "Files DIFFER: " << diff_lines_ << " lines differ, max percentage error: " << max_percentage_error_ << "%\n";
        }
    }
}

// Helper: count columns in a file (used for formatting)
uint NumericDiff::filesColumns(const std::string& file) const {
    std::ifstream fin(file);
    if (!fin.is_open()) return 0;
    std::string line;
    if (!std::getline(fin, line)) return 0;
    std::istringstream iss(line);
    std::string token;
    uint count = 0;
    while (iss >> token) ++count;
    return count;
}

// Helper: check if a string is numeric
bool NumericDiff::isNumeric(const std::string& str) const {
    char* end = nullptr;
    std::strtod(str.c_str(), &end);
    return end != str.c_str() && *end == '\0';
}

// Helper: tokenize a line into a vector of strings
static std::vector<std::string> tokenize(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Helper: calculate column widths for side-by-side output
static std::vector<size_t> calc_col_widths(const std::vector<std::string>& t1, const std::vector<std::string>& t2) {
    size_t n = std::min(t1.size(), t2.size());
    std::vector<size_t> col_widths(n, 0);
    for (size_t i = 0; i < n; ++i) {
        col_widths[i] = std::max(t1[i].size(), t2[i].size());
    }
    return col_widths;
}

// Compare two lines, print differences according to options
void NumericDiff::compareLine(const std::string& line1, const std::string& line2) const {
    // Tokenize both lines
    std::vector<std::string> tokens1 = tokenize(line1);
    std::vector<std::string> tokens2 = tokenize(line2);
    std::vector<std::string> output1, output2, errors;
    std::string toPrint1, toPrint2, toPrintErrors;

    // Calculate column widths for pretty output
    std::vector<size_t> col_widths = calc_col_widths(tokens1, tokens2);
    size_t n = col_widths.size();

    bool any_error = false;
    double max_diff_this_line = 0.0;
    for (size_t i = 0; i < n; ++i) {
        // Compare only if both tokens are numeric
        if (isNumeric(tokens1[i]) && isNumeric(tokens2[i])) {
            double v1 = std::stod(tokens1[i]);
            double v2 = std::stod(tokens2[i]);
            double diff = percentageDifference(v1, v2);
            if (std::abs(diff) > tol_) {
                any_error = true;
                if (std::abs(diff) > max_diff_this_line) max_diff_this_line = std::abs(diff);
                printRed(tokens1[i]);
                printRed(tokens2[i]);
                output1.push_back(tokens1[i]);
                output2.push_back(tokens2[i]);
                // Format the error column with right alignment
                std::ostringstream oss;
                oss << std::setw(static_cast<int>(col_widths[i])) << std::setfill(' ') << std::right << diff << "%";
                errors.push_back(oss.str());
            } else {
                output1.push_back(tokens1[i]);
                output2.push_back(tokens2[i]);
                errors.push_back(std::string(col_widths[i], ' '));
            }
        } else {
            // Non-numeric tokens are just copied
            output1.push_back(tokens1[i]);
            output2.push_back(tokens2[i]);
            errors.push_back(std::string(col_widths[i], ' '));
        }
    }

    // Join tokens for output
    auto join = [](const std::vector<std::string>& v) {
        std::string out;
        for (size_t i = 0; i < v.size(); ++i) {
            if (i > 0) out += " ";
            out += v[i];
        }
        return out;
    };
    toPrint1 = join(output1);
    toPrint2 = join(output2);
    toPrintErrors = join(errors);

    if (any_error) {
        ++diff_lines_;
        if (max_diff_this_line > max_percentage_error_) max_percentage_error_ = max_diff_this_line;
    }

    if (only_equal_) {
        // Do not print anything for individual lines in only_equal_ mode
        return;
    }

    // Print in diff style
    if (side_by_side_) {
        // Only print if any_error is true, or if suppress_common_lines_ is false
        if (any_error || !suppress_common_lines_) {
            printSideBySide(toPrint1, toPrint2);
        }
    } else {
        printDiff(toPrint1, toPrint2, toPrintErrors);
    }
}

// Calculate the percentage difference between two values
double NumericDiff::percentageDifference(double value1, double value2) const {
    if (std::abs(value1) < threshold_ && std::abs(value2) < threshold_) {
        return 0.0;
    }
    if ((std::abs(value1) < threshold_ && std::abs(value2) >= threshold_) || 
        (std::abs(value2) < threshold_ && std::abs(value1) >= threshold_)) {
        return 1.E99; // One value is below threshold, the other is not
    }
    double percentage_diff = std::abs(value1 - value2) / std::max(std::abs(value1), std::abs(value2)) * 100.0;
    if (percentage_diff < tol_) {
        return 0.0; // Values are within tolerance
    }
    return percentage_diff; // Return the percentage difference
}

// Helper to strip ANSI escape codes (color codes) from a string
std::string NumericDiff::stripAnsi(const std::string& input) const {
    std::string result;
    bool in_escape = false;
    for (size_t i = 0; i < input.size(); ++i) {
        if (!in_escape) {
            if (input[i] == '\033' && i + 1 < input.size() && input[i + 1] == '[') {
                in_escape = true;
            } else {
                result += input[i];
            }
        } else {
            // End of ANSI escape sequence is marked by 'm'
            if (input[i] == 'm') {
                in_escape = false;
            }
        }
    }
    return result;
}

// Overload printSideBySide to accept any_error
void NumericDiff::printSideBySide(const std::string& line1, const std::string& line2, bool any_error) const {
    // If suppress_common_lines_ is true, only print if any_error is true
    if (suppress_common_lines_ && !any_error) {
        return;
    }
    int width = line_length_;
    std::string l1 = line1, l2 = line2;
    std::string l1_plain = stripAnsi(l1);
    std::string l2_plain = stripAnsi(l2);
    int len1 = static_cast<int>(l1_plain.size());
    int len2 = static_cast<int>(l2_plain.size());

    // Truncate or pad l1
    if (len1 > width) {
        int count = 0;
        std::string out;
        bool in_escape = false;
        for (size_t i = 0; i < l1.size(); ++i) {
            if (!in_escape) {
                if (l1[i] == '\033' && i + 1 < l1.size() && l1[i + 1] == '[') {
                    in_escape = true;
                    out += l1[i];
                } else if (count < width) {
                    out += l1[i];
                    ++count;
                } else {
                    break;
                }
            } else {
                out += l1[i];
                if (l1[i] == 'm') {
                    in_escape = false;
                }
            }
        }
        l1 = out;
    } else if (len1 < width) {
        l1 += std::string(static_cast<std::string::size_type>(width - len1), ' ');
    }

    // Truncate or pad l2
    if (len2 > width) {
        int count = 0;
        std::string out;
        bool in_escape = false;
        for (size_t i = 0; i < l2.size(); ++i) {
            if (!in_escape) {
                if (l2[i] == '\033' && i + 1 < l2.size() && l2[i + 1] == '[') {
                    in_escape = true;
                    out += l2[i];
                } else if (count < width) {
                    out += l2[i];
                    ++count;
                } else {
                    break;
                }
            } else {
                out += l2[i];
                if (l2[i] == 'm') {
                    in_escape = false;
                }
            }
        }
        l2 = out;
    } else if (len2 < width) {
        l2 += std::string(static_cast<std::string::size_type>(width - len2), ' ');
    }

    // Decide separator: if both lines have no red color, use space, else use " | "
    bool has_red = (l1.find("\033[31m") != std::string::npos) || (l2.find("\033[31m") != std::string::npos);
    const char* sep = has_red ? "   |   " : "       ";
    std::cout << l1 << sep << l2 << "\n";
}

// Keep the original printSideBySide for backward compatibility
void NumericDiff::printSideBySide(const std::string& line1, const std::string& line2) const {
    printSideBySide(line1, line2, true);
}

// Print differences in a diff-like format
void NumericDiff::printDiff(const std::string& output1, const std::string& output2, const std::string& errors) const {
    // Only print lines that contain red marks (i.e., differences)
    bool has_red1 = output1.find("\033[31m") != std::string::npos;
    bool has_red2 = output2.find("\033[31m") != std::string::npos;
    if (has_red1 || has_red2) {
        std::cout << '\n';
        std::cout << "< " << output1 << "\n";
        std::cout << "> " << output2 << "\n";
        std::cout << ">>" << errors << "\n";
    }
}