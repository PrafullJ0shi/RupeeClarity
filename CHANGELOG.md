# Changelog

All notable changes to this project are documented in this file.

## [1.2.0] - 2026-11-22

### Fixed
- Input is read as whole lines with `fgets` and checked with `strtol` / `strtod`. Menu choices such as `2.5` or `12abc`, numbers such as `12abc`, `inf`, `nan`, `1e999` and `0x10`, empty lines and over-long lines are rejected with a message instead of being partly accepted.
- One ENTER now returns to the menu after every module.
- End of input (Ctrl+D, Ctrl+Z or the end of piped input) prints a message and exits cleanly instead of looping.

### Added
- Browser version in `web/`: the same C source compiled to WebAssembly with Emscripten, running in an xterm.js terminal with an input box and Enter button for phones, one-tap menu buttons and a report download. Published to <https://prafullj0shi.github.io/RupeeClarity/> by `.github/workflows/pages.yml`.
- `.github/workflows/ci.yml`: compiles with `-Wall -Wextra -Werror` on Linux, macOS and Windows and runs the tests on every push.
- `.github/workflows/release.yml`: on a `v*` tag, builds ready-to-run programs for Windows x64, macOS (universal), Linux x64 and ARM64, and Android ARM64 and ARMv7 (Termux), and attaches them with checksums to a GitHub Release.
- `install.sh` (Linux, macOS, Android/Termux) and `install.ps1` (Windows) one-line installers.
- `tests/test_cli.py`: scripted tests of all modules, invalid input and end of input.
- README: a "Run in browser" button, CI and release badges, and a "Download / Install" section with per-OS steps.

### Changed
- README "Known limitations" and "Roadmap", and the project report, updated for the fixes above.

## [1.1.0] - 2026-11-22

### Added
- `docs/demo/RupeeClarity_Demo.mp4`: an 8 min 47 s, 1080p demo recorded from a real terminal session. It shows the build and every module from 1 to 11, including all four compounding frequencies, Goal Check with three outcomes, the saved report file, input validation and exit.
- `docs/demo/demo-preview.gif`: a short preview of the main menu, Compare SI and CI, and the Year-by-Year Table, embedded at the top of the README.
- `.devcontainer/devcontainer.json`: GitHub Codespaces setup with GCC preinstalled. It builds the program when the codespace is created.
- README: an "Open in GitHub Codespaces" badge, a demo video chapter list, and a "Run it online (no install)" guide for Codespaces, OnlineGDB and Programiz.

## [1.0.0] - 2026-11-22

### Added
- `src/rupeeclarity.c`: menu-driven SI/CI simulator with 11 modules (Simple Interest Lab, Compound Interest Lab, Compare SI and CI, Year-by-Year Table, Goal Check, What-If Rate +1%, Years to Double, Story Mode, Save Report File, Formula Vault, About / Project Card).
- Project report (`docs/RupeeClarity_Project_Report.docx` and `.pdf`), console screenshots and charts.
- Sample output of module 9 (`samples/sample_report.txt`).
- README, MIT license and this changelog.

### Changed
- Formula Vault: `Rate (%)` in the `printf` format string is now written as `Rate (%%)`, so the percent sign prints portably and `gcc -Wall -Wextra` builds with no warnings.
- Public edition: the header comment and the About screen show the Class Roll No. only; other identification numbers from the college submission are omitted.
