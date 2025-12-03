#!/usr/bin/env bash
# ======================================================================
# perf_setup.sh
# Prepares environment for CPU profiling on Ubuntu 24.04 + HWE kernel.
# Must be executed BEFORE running profiling (perf_flamegraph.sh, perf_hotspot.sh).
# ======================================================================

set -e

echo "=== QuantumGradesApp — perf profiling setup ==="

# ----------------------------------------------------------------------
# 1. Validate `perf` alias
# ----------------------------------------------------------------------
PERF="/usr/lib/linux-tools-6.8.0-88/perf"

echo "[1/4] Checking perf alias..."

# Pobieramy alias w formacie zgodnym z Bash i ZSH
ALIASED_PATH="$(alias perf 2>/dev/null | sed -E "s/alias perf='([^']*)'/\1/")"

if [[ ! -x "$PERF" ]]; then
    echo "ERROR: expected perf binary does not exist:"
    echo "       $PERF"
    echo "Install via:"
    echo "       sudo apt install linux-tools-generic linux-tools-$(uname -r)"
    exit 1
fi

echo "Using perf binary: $PERF"

# ----------------------------------------------------------------------
# 2. Lower paranoia level
# ----------------------------------------------------------------------
echo "[2/4] Checking perf_event_paranoid..."
sudo sysctl -w kernel.perf_event_paranoid=1

# ----------------------------------------------------------------------
# 3. Allow resolving kernel symbols
# ----------------------------------------------------------------------
echo "[3/4] Checking kptr_restrict..."
sudo sysctl -w kernel.kptr_restrict=0

# ----------------------------------------------------------------------
# 4. Ensure build-id cache exists
# ----------------------------------------------------------------------
echo "[4/4] Ensuring buildid cache directory exists..."
mkdir -p ~/.debug
chmod 755 ~/.debug

echo "=== perf setup complete ==="
