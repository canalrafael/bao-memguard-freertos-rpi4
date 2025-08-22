#!/bin/bash

# This script fully automates the testing process for all 960 benchmarks.
# For each index, it dynamically creates a robust minicom script, runs the test,
# and captures the output to a unique log file.

# --- Configuration ---
LOG_DIR="bench_logs"
TEMP_SCRIPT_FILE="temp_loader.script"

# --- Setup ---
mkdir -p "$LOG_DIR"

# --- Main Loop ---
for i in $(seq 1 160); do
    LOG_FILE="$LOG_DIR/bao-$i.log"
    
    echo "========================================================"
    echo "===== Preparing and running test for bao-$i.bin ====="
    echo "========================================================"

    # Step 1: Dynamically create a more robust minicom script.
    cat > "$TEMP_SCRIPT_FILE" << EOF
        # Set a 30-second timeout for all expect commands.
        timeout 30
        verbose on

        # Add print statements for better debugging.
        print "--> Waiting for autoboot prompt..."
        # Wait for a more generic part of the autoboot prompt.
        expect "autoboot:"
        send " "
        print "--> Autoboot stopped. Waiting for U-Boot prompt..."

        # Wait for the main U-Boot prompt.
        expect "U-Boot>"
        # Send the command with the correctly substituted index.
        send "fatload mmc 0 0x200000 bao-${i}.bin; go 0x200000"
        print "--> Benchmark command sent. Waiting for completion..."

        # Wait for your C program to print the completion message.
        expect "end iteration"
        print "--> Completion message received. Exiting."

        # Forcefully quit minicom to end this iteration.
        ! pkill minicom
EOF

    # Step 2: Run minicom using the script.
    minicom -o -C "$LOG_FILE" -S "$TEMP_SCRIPT_FILE"

    if [ $? -eq 0 ]; then
        echo "===== Finished test for bao-$i.bin, log saved to $LOG_FILE ====="
    else
        echo ">>>>> WARNING: Minicom may not have exited cleanly for index $i. <<<<<"
    fi
    echo ""

done

# --- Cleanup ---
rm -f "$TEMP_SCRIPT_FILE"

echo "========================================================"
echo "All tests completed."
echo "========================================================"
