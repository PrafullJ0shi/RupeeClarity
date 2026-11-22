#!/usr/bin/env bash
# Builds the browser (WebAssembly) version of RupeeClarity into web/.
# Needs Emscripten (emcc) on PATH: https://emscripten.org/docs/getting_started/
set -euo pipefail
cd "$(dirname "$0")"
emcc ../src/rupeeclarity.c -O2 -std=c99 -Wall -Wextra \
  -o rupeeclarity.js \
  -sASYNCIFY \
  -sMODULARIZE=1 -sEXPORT_NAME=createRupeeClarity \
  -sENVIRONMENT=web \
  -sEXIT_RUNTIME=1 \
  -sFORCE_FILESYSTEM=1 \
  -sEXPORTED_RUNTIME_METHODS=FS \
  -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$stringToUTF8,$lengthBytesUTF8' \
  -lm
echo "Built web/rupeeclarity.js and web/rupeeclarity.wasm"
