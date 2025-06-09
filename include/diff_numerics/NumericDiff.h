// NumericDiff.h
// -------------------------------------------------------------
// This header defines the NumericDiff class, which provides functionality
// for comparing two numerical data files line by line, with configurable
// tolerance, threshold, and output options.
//
// The class is used by the diff-numerics command-line tool and in tests.
// -------------------------------------------------------------

#pragma once
#include <string>

class NumericDiff {
public:
    // Constructor: set up comparison options and file paths
    NumericDiff(const std::string& file1, const std::string& file2, double tol, double threshold, bool side_by_side, const std::string& comment_char = "#", int line_length = 60, bool suppress_common_lines = false, bool only_equal = false, bool quiet = false);
    // Run the comparison and print results according to options
    void run();
private:
    // File paths and options
    std::string file1_;
    std::string file2_;
    double tol_;
    double threshold_;
    bool side_by_side_;
    std::string comment_char_;
    int line_length_;
    // suppress_common_lines_ only suppresses common lines if explicitly set (not by -y/side_by_side alone)
    bool suppress_common_lines_;
    bool only_equal_;
    bool quiet_;
private:
    // Helper: count columns in a file
    uint filesColumns(const std::string& file) const;
    // Helper: check if a line is a comment
    inline bool isLineComment(const std::string& line) const {
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) return false;
        return line.compare(pos, comment_char_.size(), comment_char_) == 0;
    }
    // Helper: check if a line contains a comment but is not just a comment
    inline bool lineContainsComment_but_isNotJustComment(const std::string& line) const {
        size_t pos = line.find(comment_char_);
        return pos != std::string::npos && pos != 0 && !line.substr(0, pos).empty();
    }
    // Helper: remove comment from a line
    inline std::string removeComment(const std::string& line) const {
        size_t pos = line.find(comment_char_);
        return (pos == std::string::npos) ? line : line.substr(0, pos);
    }
    // Helper: check if a string is numeric
    bool isNumeric(const std::string& str) const;
    // Compare two lines and print results
    void compareLine(const std::string& line1, const std::string& line2) const;
    // Compute percentage difference between two values
    double percentageDifference(double value1, double value2) const;
    // Compare two values (not used directly)
    void compareValues(double value1, double value2) const;
    // Print diff output in various formats
    void printDiff(const std::string& line1, const std::string& line2, const std::string& errors) const;
    void printSideBySide(const std::string& line1, const std::string& line2) const;
    // Print a string in red (for errors)
    inline void printRed(std::string& str) const {
        // Placeholder for red text output, e.g., using ANSI escape codes
        str = "\033[31m" + str + "\033[0m"; // Red color
    }
    // Remove ANSI color codes from a string
    std::string stripAnsi(const std::string& input) const;

    // For summary/statistics
    mutable size_t diff_lines_ = 0;
    mutable double max_percentage_error_ = 0.0;
};
