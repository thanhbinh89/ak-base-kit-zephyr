#!/usr/bin/env bash
# Analyse the flash (ROM) and RAM footprint of the current build.
#
#   tools/footprint.sh            # text reports + sunburst plots + HTML dashboard
#   tools/footprint.sh --serve    # ...and serve the dashboard on localhost:8000
#
# Outputs, all under build/:
#   rom.json / ram.json        machine-readable size trees
#   rom-sunburst.html          interactive sunburst chart (plotly)
#   ram-sunburst.html
#   dashboard/index.html       full dashboard: memory, Kconfig, devicetree, sysinit
#
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
ZEPHYRPROJECT="${ZEPHYRPROJECT:-$HOME/zephyrproject}"
ZEPHYR_BASE="$ZEPHYRPROJECT/zephyr"
PYTHON="$ZEPHYRPROJECT/.venv/bin/python"
[ -x "$PYTHON" ] || PYTHON="$(command -v python3)"

[ -f "$BUILD_DIR/zephyr/zephyr.elf" ] || {
	echo "error: no build in $BUILD_DIR -- run tools/build.sh first" >&2
	exit 1
}

export PATH="$ZEPHYRPROJECT/.venv/bin:$PATH"

echo "==> text reports (also writes rom.json / ram.json)"
ninja -C "$BUILD_DIR" rom_report ram_report

echo "==> sunburst charts"
for r in rom ram; do
	"$PYTHON" "$ZEPHYR_BASE/scripts/footprint/plot.py" \
		"$BUILD_DIR/$r.json" --html "$BUILD_DIR/$r-sunburst.html"
	echo "    $BUILD_DIR/$r-sunburst.html"
done

echo "==> HTML dashboard"
"$PYTHON" "$ZEPHYR_BASE/scripts/dashboard/dashboard.py" \
	--output "$BUILD_DIR/dashboard" \
	--zephyr-base "$ZEPHYR_BASE" \
	--kernel-bin-name zephyr \
	"$BUILD_DIR"

echo
echo "dashboard: $BUILD_DIR/dashboard/index.html"

if [ "${1:-}" = "--serve" ]; then
	echo "serving on http://127.0.0.1:8000/ (Ctrl+C to stop)"
	exec "$PYTHON" -m http.server 8000 --directory "$BUILD_DIR/dashboard"
fi
