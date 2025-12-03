#!/usr/bin/env bash
# perf_hotspot.sh APP_PATH CONFIG_PATH
set -e

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <app> <config>"
    exit 1
fi

APP="$1"
CONFIG="$2"
PERF="/usr/lib/linux-tools-6.8.0-88/perf"

OUT_DIR="profiling_output"
mkdir -p "$OUT_DIR"

RAW_FILE="$OUT_DIR/perf_hotspot.data"
REPORT="$OUT_DIR/perf_hotspot_report.txt"

echo "[1/2] Recording perf data..."
$PERF record --all-user-F 199 -g -o "$RAW_FILE" -- "$APP" --config "$CONFIG"

echo "[2/2] Generating hotspot report..."
$PERF report --all-user -i "$RAW_FILE" --stdio > "$REPORT"

echo "Done. Hotspot report: $REPORT"
