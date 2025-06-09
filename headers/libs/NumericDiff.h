#pragma once
#include <string>

class NumericDiff {
public:
    NumericDiff(const std::string& file1, const std::string& file2, double tol, double threshold, bool side_by_side, const std::string& comment_char = "#", int line_length = 60, bool suppress_common_lines = false, bool only_equal = false, bool quiet = false);
    void run();
private:
    std::string file1_;
    std::string file2_;
    double tol_;
    double threshold_;
    bool side_by_side_;
    std::string comment_char_;
    int line_length_;
    bool suppress_common_lines_;
    bool only_equal_;
    bool quiet_;
    // Add helper methods and members as needed
private:
    uint filesColumns(const std::string& file) const;
    inline bool isLineComment(const std::string& line) const {
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) return false;
        return line.compare(pos, comment_char_.size(), comment_char_) == 0;
    }
    inline bool lineContainsComment_but_isNotJustComment(const std::string& line) const {
        size_t pos = line.find(comment_char_);
        return pos != std::string::npos && pos != 0 && !line.substr(0, pos).empty();
    }
    inline std::string removeComment(const std::string& line) const {
        size_t pos = line.find(comment_char_);
        return (pos == std::string::npos) ? line : line.substr(0, pos);
    }
    bool isNumeric(const std::string& str) const;
    void compareLine(const std::string& line1, const std::string& line2) const;
    double percentageDifference(double value1, double value2) const;
    void compareValues(double value1, double value2) const;
    void printDiff(const std::string& line1, const std::string& line2, const std::string& errors) const;
    void printSideBySide(const std::string& line1, const std::string& line2, bool any_error) const;
    void printSideBySide(const std::string& line1, const std::string& line2) const;
    inline void printRed(std::string& str) const {
        // Placeholder for red text output, e.g., using ANSI escape codes
        str = "\033[31m" + str + "\033[0m"; // Red color
    }
    std::string stripAnsi(const std::string& input) const;

    // For summary/statistics
    mutable size_t diff_lines_ = 0;
    mutable double max_percentage_error_ = 0.0;
};
