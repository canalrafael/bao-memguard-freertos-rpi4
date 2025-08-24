#!/bin/bash

# This script fully automates the testing process for all 160 benchmarks.
# For each index, it dynamically creates a robust minicom script, runs the test,
# and captures the output to a unique log file.

# --- Configuration ---
LOG_DIR="benchmarks/logs"
TEMP_SCRIPT_FILE="benchmarks/temp_loader.script"

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

        expect {
            "end iteration" {
                print "--> ✅ Completion message received. Exiting."
                # Use pkill to forcefully and reliably exit minicom.
                ! pkill minicom
            }
            timeout {
                print "--> ❌ TIMEOUT: 'end iteration' string not found. Exiting."
                # Also use pkill on timeout.
                ! pkill minicom
            }
        }
EOF

    # Step 2: Run minicom. The script will always be killed by pkill.
    minicom -o -C "$LOG_FILE" -S "$TEMP_SCRIPT_FILE"

    # Step 3: Check the log file to determine if the test passed or failed.
    # We use 'grep -q' which is quiet and returns a success exit code (0) if the text is found.
    if grep -q "✅ Completion message received" "$LOG_FILE"; then
        echo "===== Finished test for bao-$i.bin, log saved to $LOG_FILE ====="
    else
        echo ">>>>> ❌ ERROR: Test for index $i failed (timeout). Stopping automation. <<<<<"
        echo ">>>>> Check the log file for details: $LOG_FILE <<<<<"
        break # Exit the for loop.
    fi
    echo ""

done

# --- Cleanup ---
rm -f "$TEMP_SCRIPT_FILE"

echo "========================================================"
echo "Automation script finished."
echo "========================================================"
