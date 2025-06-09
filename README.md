# diff-numerics

A command-line tool for comparing numerical data files with configurable tolerance, threshold, and output formatting. Designed for scientific and engineering workflows where small floating-point differences are expected.

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
./bin/diff_numerics [options] file1 file2
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

### Example

```bash
./bin/diff_numerics -y -t 0.01 -T 1e-5 data1.dat data2.dat
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
./bin/diff_numerics -y delta_3D2_2.dat delta_3D2.dat
```

---

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, new features, or documentation improvements.

---

## License

This project is licensed under the terms of the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

---

## TODO / Ideas

- Realign lines whose number are different in the two files but are otherwise similar.
- Add unit tests and CI integration.
- Support for config files.
- Output to HTML/Markdown.
- GUI frontend.
- Improved error handling and reporting.
