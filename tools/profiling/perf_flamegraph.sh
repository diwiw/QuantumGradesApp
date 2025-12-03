#!/usr/bin/env bash
# perf_flamegraph.sh APP_PATH CONFIG_PATH
set -e

APP="$1"
CONFIG="$2"
PERF="/usr/lib/linux-tools-6.8.0-88/perf"

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <app> <config>"
    exit 1
fi

OUT_DIR="profiling_output"
RAW_FILE="$OUT_DIR/perf_flamegraph.data"
FOLDED="$OUT_DIR/flamegraph.folded"
SVG="$OUT_DIR/flamegraph.svg"


mkdir -p "$OUT_DIR"

echo "[1/3] Recording perf data (userspace only)..."
$PERF record --all-user -F 199 -g --call-graph dwarf -o "$RAW_FILE" -- "$APP" --config "$CONFIG"

echo "[2/3] Converting perf.data -> folded..."
$PERF script -i "$RAW_FILE"

echo "[3/3] Generating flamegraph..."
flamegraph.pl "$FOLDED" > "$SVG"

echo "Done. Flamegraph: $SVG"
