#!/usr/bin/env bash
# Build the AK Base Kit firmware without having to set anything up first.
#
#   tools/build.sh                # incremental build of app/
#   tools/build.sh -p always      # pristine build
#   tools/build.sh -t menuconfig  # any other west build argument passes through
#
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ZEPHYRPROJECT="${ZEPHYRPROJECT:-$HOME/zephyrproject}"

export ZEPHYR_BASE="$ZEPHYRPROJECT/zephyr"
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR="${ZEPHYR_SDK_INSTALL_DIR:-$HOME/zephyr-sdk-1.0.1}"
export PATH="/home/binh/tools/STM32CubeProgrammer/bin:$PATH"

WEST="$ZEPHYRPROJECT/.venv/bin/west"
[ -x "$WEST" ] || WEST="$(command -v west)"

exec "$WEST" build -b ak_base_kit "$PROJECT_ROOT/app" \
	-d "$PROJECT_ROOT/build" "$@"
