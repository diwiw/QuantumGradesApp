#!/usr/bin/env bash
set -e

CLI_BIN=$1
CONFIG=$2

if [ -z "$CLI_BIN" ] || [ -z "$CONFIG" ]; then
    echo "Usage: $0 <path-to-cli-binary> <build-configuration>"
    exit 1
fi

OUT_DIR="perf_out"
RAW_FILE="$OUT_DIR/perf_data_${CONFIG}.data"

mkdir -p "$OUT_DIR"

echo "[*] Running perf..."
sudo perf record -F 99 -g -o "$RAW_FILE" -- "$CLI_BIN" --config "$CONFIG"

echo "[*] Perf data saved to: $RAW_FILE"