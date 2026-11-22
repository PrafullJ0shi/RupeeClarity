#!/usr/bin/env bash
# RupeeClarity one-line installer for Linux, macOS and Android (Termux).
#   curl -fsSL https://raw.githubusercontent.com/PrafullJ0shi/RupeeClarity/main/install.sh | bash
# Downloads the ready-made program from the latest GitHub Release, installs it
# as "rupeeclarity" and starts it. Set RC_NO_RUN=1 to install without starting.
set -eu

REPO="PrafullJ0shi/RupeeClarity"
BASE="${RC_BASE_URL:-https://github.com/$REPO/releases/latest/download}"

say()  { printf '\033[1;32m==>\033[0m %s\n' "$*"; }
fail() { printf '\033[1;31mError:\033[0m %s\n' "$*" >&2; exit 1; }

os="$(uname -s)"
arch="$(uname -m)"
termux=0
prefix="${PREFIX:-}"
case "$prefix" in *com.termux*) termux=1 ;; esac
[ -n "${TERMUX_VERSION:-}" ] && termux=1

case "$os" in
  Linux)
    if [ "$termux" = 1 ] || [ "$(uname -o 2>/dev/null)" = "Android" ]; then
      case "$arch" in
        aarch64|arm64)        asset="rupeeclarity-android-arm64" ;;
        armv7l|armv8l|arm*)   asset="rupeeclarity-android-armv7" ;;
        *) fail "this Android phone ($arch) is not supported yet. Use https://prafullj0shi.github.io/RupeeClarity/ instead." ;;
      esac
    else
      case "$arch" in
        x86_64|amd64)  asset="rupeeclarity-linux-x64" ;;
        aarch64|arm64) asset="rupeeclarity-linux-arm64" ;;
        *) fail "Linux on $arch is not supported yet. Build from source: gcc src/rupeeclarity.c -o rupeeclarity -lm" ;;
      esac
    fi ;;
  Darwin) asset="rupeeclarity-macos-universal" ;;
  MINGW*|MSYS*|CYGWIN*)
    fail "on Windows, download https://github.com/$REPO/releases/latest/download/rupeeclarity-windows-x64.exe and double-click it." ;;
  *) fail "unsupported system: $os $arch" ;;
esac

if [ "$termux" = 1 ]; then
  bindir="$prefix/bin"
elif [ -w /usr/local/bin ] 2>/dev/null; then
  bindir="/usr/local/bin"
else
  bindir="$HOME/.local/bin"
fi
mkdir -p "$bindir"
dest="$bindir/rupeeclarity"

say "Downloading $asset"
tmp="$(mktemp "${TMPDIR:-/tmp}/rupeeclarity.XXXXXX")"
if command -v curl >/dev/null 2>&1; then
  curl -fL --progress-bar "$BASE/$asset" -o "$tmp" || fail "download failed. Check your internet connection."
elif command -v wget >/dev/null 2>&1; then
  wget -q --show-progress -O "$tmp" "$BASE/$asset" || fail "download failed. Check your internet connection."
else
  fail "curl or wget is needed (Termux: pkg install curl)."
fi
chmod 755 "$tmp"
mv -f "$tmp" "$dest"
[ "$os" = "Darwin" ] && xattr -d com.apple.quarantine "$dest" 2>/dev/null || true

say "Installed to $dest"
case ":$PATH:" in
  *":$bindir:"*) say "Next time just type: rupeeclarity" ;;
  *) say "Next time run: $dest   (or add $bindir to your PATH)" ;;
esac

if [ "${RC_NO_RUN:-0}" != 1 ]; then
  # The script itself arrives through a pipe, so give the program the keyboard.
  if [ -r /dev/tty ] && { : </dev/tty; } 2>/dev/null; then
    say "Starting RupeeClarity..."
    cd "$HOME"
    exec "$dest" </dev/tty
  else
    say "Run it with: $dest"
  fi
fi
