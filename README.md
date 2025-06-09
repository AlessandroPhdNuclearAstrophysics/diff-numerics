# diff-numerics

---
**Project Overview**

`diff-numerics` is a professional C++ tool for comparing numerical data files with configurable tolerance, threshold, and output options. It is designed for scientific and engineering workflows where precise numerical comparison is required.

- **Source code:** See `src/` and `include/diff-numerics/`
- **Tests:** See `test/` (uses GoogleTest)
- **Build system:** CMake and Makefile (see below)
- **Documentation:** Man page (`diff-numerics.1`), this README, and code comments

---

## Project Structure

- `src/` - Main source files for the diff-numerics tool
- `include/diff-numerics/` - Public headers (notably `NumericDiff.h`)
- `test/` - Test suite, test data, and test CMake configuration
- `bin/` - Built executables
- `build/` - CMake build directory (not in version control)
- `Makefile` - Top-level automation for build, install, test, and clean
- `CMakeLists.txt` - Top-level CMake configuration
- `diff-numerics.1` - Man page for the command-line tool

---

## Building and Installing

### With Makefile (recommended)

```sh
make           # Build the project
make test      # Run the test suite
sudo make install   # Install binary, headers, and man page
sudo make uninstall # Remove installed files
make clean     # Remove build artifacts
```

### With CMake directly

```sh
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
sudo cmake --install build
```

---

## Testing

- Tests are written with GoogleTest and located in `test/test_diff-numerics.cpp`.
- Test data files are in `test/`.
- Run `make test` or `ctest` from the build directory to execute all tests.
- To run a specific test case or see verbose output, use:

```sh
cd build
./bin/diff-numerics_tests --gtest_filter=DiffNumerics.*
```

- To manually check the CLI output for a specific test case (e.g., for the 3P2-3F2 files):

```sh
./bin/diff-numerics test/delta_3P2-3F2.dat test/delta_3P2-3F2_2.dat
./bin/diff-numerics -y test/delta_3P2-3F2.dat test/delta_3P2-3F2_2.dat
./bin/diff-numerics -s test/delta_3P2-3F2.dat test/delta_3P2-3F2_2.dat
```

- The test suite covers all main diff-numerics modes (default, tight tolerance, side-by-side, suppress common lines, quiet, CLI summary) and asserts on output patterns for robust coverage.

---

## Contributing

- See `CONTRIBUTING.md` for guidelines.
- Code style is enforced via `.clang-format` and `.editorconfig`.
- Please read the comments in each source file for guidance on structure and intent.

---

## Documentation

- The code is heavily commented for clarity and onboarding.
- See the man page (`diff-numerics.1`) for command-line usage.
- For more details, see the comments at the top of each source, header, and build file.

---

## License

See `LICENSE` for details.

---

## TODO / Ideas

- Add option to ignore columns that are zero in both files for each line.
- Realign lines whose numbers are different in the two files but are otherwise similar.
- Add unit tests and CI integration.
- Support for config files.
- Output to HTML/Markdown.
- GUI frontend.
- Improved error handling and reporting.

---

## Features

- Compares two files line by line, column by column.
- Supports floating-point tolerance and threshold for ignoring insignificant differences.
- Side-by-side diff output, with optional suppression of common lines.
- Customizable comment character to skip metadata or header lines.
- Quiet and summary-only modes for scripting and automation.
- Colorized output for easy identification of differences.

---

## Build & Installation

### Prerequisites

- C++ compiler (C++11 or newer)
- CMake (version 3.10+ recommended)
- Make

### Build Steps

```bash
# Clone the repository (if not already)
git clone <repo-url>
cd diff-numerics

# Build using Makefile (uses CMake under the hood)
make
```

The binary will be placed in the `bin/` directory.

---

## Usage

```bash
./bin/diff-numerics [options] file1 file2
```

### Options

| Option                        | Description                                                                 |
|-------------------------------|-----------------------------------------------------------------------------|
| `-y`, `--side-by-side`        | Print lines side by side                                                    |
| `-ys`, `--suppress-common-lines` | Suppress lines within tolerance in side-by-side mode (implies -y)         |
| `-t`, `--tolerance <value>`   | Tolerance for percentage difference (default: 1E-2)                         |
| `-T`, `--threshold <value>`   | Ignore if both values are under threshold (default: 1E-6)                   |
| `-c`, `--comment-string <str>`| String to start a comment (default: `#`)                                    |
| `-w`, `--single-column-width <n>` | Set column width for side-by-side output (default: 60)                  |
| `-s`, `--report-identical-files` | Print only if files are equal within tolerance, otherwise print summary   |
| `-q`, `--quiet`               | Suppress all output if files are equal within tolerance                     |
| `-d`, `--color-different-digits` | Colorize only the part of the numbers that differ                        |
| `-C`, `--columns <list>`         | Comma-separated list of columns (0-based) to compare                     |

### Example

```bash
./bin/diff-numerics -y -d -C 2,3 -t 0.01 -T 1e-5 data1.dat data2.dat
```

---

## Output

- **Side-by-side mode:** Shows both files' lines next to each other, highlighting differences.
- **Suppressed common lines:** Only lines with differences are shown.
- **Quiet mode:** No output if files are equal within tolerance.
- **Summary mode:** Prints a summary if files are not equal.

---

## Test

A sample test output is provided in the `test` file. You can generate your own by running:

```bash
./bin/diff-numerics -y delta_3D2_2.dat delta_3D2.dat
```

---

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, new features, or documentation improvements.

---

## License

This project is licensed under the terms of the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

---

## TODO / Ideas

- Give the possibility to ignore the columns that for both files are zero for each line.
- Realign lines whose number are different in the two files but are otherwise similar.
- Support for config files.
- Output to HTML/Markdown.
- GUI frontend.
- Improved error handling and reporting.
