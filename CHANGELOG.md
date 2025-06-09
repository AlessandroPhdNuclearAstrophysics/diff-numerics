# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]
- Initial professionalization of project structure and documentation.
- Added `-d, --color-different-digits` flag: colorizes only the part of numbers that differ, including all remaining digits and exponent after the first difference.
- Improved digit-diff coloring logic: once a difference is found in the mantissa, all remaining digits and the exponent are colored red, even if the exponent is the same.
- Updated TODO/Ideas in README: clarified and deduplicated items, added planned feature to ignore columns that are zero in both files for each line.
