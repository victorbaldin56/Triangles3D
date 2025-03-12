#!/bin/bash
set -e

INPUT_DIR="input"
MAX_TIME=1000  # 1 second threshold

for input in "$INPUT_DIR"/*.in; do
  echo "Testing $input..."
  start=$(date +%s%3N)
  $APP < "$input" > /dev/null
  elapsed=$(echo "$(date +%s%3N) - $start" | bc)
  echo "$elapsed ms"

  # Check execution time
  if (( $(echo "$elapsed > $MAX_TIME" | bc -l) )); then
    echo "$input exceeded time limit: ${elapsed}s"
    exit 1
  fi

done
