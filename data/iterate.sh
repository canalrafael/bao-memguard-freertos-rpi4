#!/bin/bash

# This script automates the entire benchmark configuration, build, and copy process.
# It iterates through a config file, and for each scenario it:
# 1. Calls the 'change' script to modify the C header files.
# 2. Builds all VMs and the Bao hypervisor.
# 3. Copies the resulting binary to data/bin/bao-{INDEX}.bin.

set -e # Exit immediately if a command exits with a non-zero status.

# --- Configuration ---
CHANGE_SCRIPT="data/change"
VM_CONFIG_FILE="data/vms_on_off.txt"
DESTINATION_DIR="data/bin"

# --- Pre-run Checks ---
if [ ! -x "$CHANGE_SCRIPT" ]; then
    echo "Error: Change script not found or not executable at '$CHANGE_SCRIPT'"
    exit 1
fi
if [ ! -r "$VM_CONFIG_FILE" ]; then
    echo "Error: VM config file not found or not readable at '$VM_CONFIG_FILE'"
    exit 1
fi

# --- Setup ---
# Source the environment file once to get necessary variables.
VM_INDEX=NULL source env.bash
# Create the destination directory for the binaries if it doesn't exist.
mkdir -p "$DESTINATION_DIR"

# --- Main Loop ---
index=1
while IFS= read -r line; do
    if [ "$index" -gt 160 ]; then
        break
    fi

    vm_states=$(echo "$line" | tr -d '<>')

    if [ -z "$vm_states" ]; then
        echo "Warning: Skipping empty line at index $index"
        continue
    fi

    echo "============================================================"
    echo "Processing Benchmark Index: $index"
    echo "============================================================"

    # Step 1: Configure the source files for the current index.
    echo "--> Configuring source for index $index..."
    "$CHANGE_SCRIPT" "$index" $vm_states

    # Step 2: Build the VMs.
    echo "--> Building all VMs for index $index..."
    sh/build_vm.sh all > /dev/null && echo "Ok"

    # Step 3: Build the Bao hypervisor.
    echo "--> Building Bao hypervisor for index $index..."
    sh/build_bao.sh > /dev/null 2>&1 && echo "Ok"

    # Step 4: Copy the resulting binary with the correct versioned name.
    SOURCE_BINARY="$BAO_DEMOS_WRKDIR_IMGS/bao.bin"
    DESTINATION_FILE="$DESTINATION_DIR/bao-$index.bin"
    echo "--> Copying '$SOURCE_BINARY' to '$DESTINATION_FILE'..."
    cp "$SOURCE_BINARY" "$DESTINATION_FILE"

    index=$((index + 1))

done < "$VM_CONFIG_FILE"

echo ""
echo "============================================================"
echo "Automation script finished. All scenarios processed."
echo "============================================================"

