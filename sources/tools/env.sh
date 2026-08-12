#!/usr/bin/env bash
# Source this to get a shell configured for building the AK Base Kit firmware:
#
#   source tools/env.sh
#   west build -b ak_base_kit app
#
# It activates the Zephyr virtualenv and points the build at the right SDK.

ZEPHYRPROJECT="${ZEPHYRPROJECT:-$HOME/zephyrproject}"

export ZEPHYR_BASE="$ZEPHYRPROJECT/zephyr"
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR="${ZEPHYR_SDK_INSTALL_DIR:-$HOME/zephyr-sdk-1.0.1}"

if [ -f "$ZEPHYRPROJECT/.venv/bin/activate" ]; then
	# shellcheck disable=SC1091
	source "$ZEPHYRPROJECT/.venv/bin/activate"
else
	echo "warning: no venv at $ZEPHYRPROJECT/.venv" >&2
fi

echo "ZEPHYR_BASE            = $ZEPHYR_BASE"
echo "ZEPHYR_SDK_INSTALL_DIR = $ZEPHYR_SDK_INSTALL_DIR"
