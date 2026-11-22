# Changelog

All notable changes to this project are documented in this file.

## [1.0.0] - 2026-11-22

### Added
- `src/rupeeclarity.c`: menu-driven SI/CI simulator with 11 modules (Simple Interest Lab, Compound Interest Lab, Compare SI and CI, Year-by-Year Table, Goal Check, What-If Rate +1%, Years to Double, Story Mode, Save Report File, Formula Vault, About / Project Card).
- Project report (`docs/RupeeClarity_Project_Report.docx` and `.pdf`), console screenshots and charts.
- Sample output of module 9 (`samples/sample_report.txt`).
- README, MIT license and this changelog.

### Changed
- Formula Vault: `Rate (%)` in the `printf` format string is now written as `Rate (%%)`, so the percent sign prints portably and `gcc -Wall -Wextra` builds with no warnings.
- Public edition: the header comment and the About screen show the Class Roll No. only; other identification numbers from the college submission are omitted.
