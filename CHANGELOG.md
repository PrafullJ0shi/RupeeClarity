# Changelog

All notable changes to this project are documented in this file.

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
