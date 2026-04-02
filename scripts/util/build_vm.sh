#!/usr/bin/env bash

input=$1

VM_NAME_PREFIX="vm_"
build_all=false

# Check if input is a number from 0 to 3
if [[ "$input" =~ ^[0-3]$ ]]; then
  VM_INDEX="$input"
  echo "Building VM index: $VM_INDEX"
elif [ "$input" == "all" ]; then
  build_all=true
  echo "Building all VMs..."
elif [ -n "$input" ]; then
  echo "Error: Invalid input. Must be 0-3 or 'all'." >&2
  exit 1
else
  echo "Error: No input provided." >&2
  exit 1
fi

# VM0 is FreeRTOS, VMs 1/2/3 are Linux
FREERTOS_VMS="0"

is_freertos_vm() {
  local idx=$1
  for fvm in $FREERTOS_VMS; do
    if [ "$fvm" == "$idx" ]; then
      return 0
    fi
  done
  return 1
}

build() {
  local idx=$1
  VM_INDEX=$idx source ./env.bash

  local VM_DIR="$BAO_DEMOS_WRKDIR_SRC/VMs/${VM_NAME_PREFIX}${idx}"
  local VM_BUILD_DIR="$VM_DIR/build/$PLATFORM"

  if is_freertos_vm "$idx"; then
    echo "=== Building FreeRTOS VM $idx ==="
    make -C "$VM_DIR" PLATFORM="$PLATFORM" $FREERTOS_PARAMS
  else
    echo "=== Building Linux VM $idx ==="
    make -C "$VM_DIR" PLATFORM="$PLATFORM"
  fi

  # Copy the built binary to the final images directory
  local BIN_FILE="$VM_BUILD_DIR/${VM_NAME_PREFIX}${idx}.bin"
  if [ -f "$BIN_FILE" ]; then
    echo "Copying $BIN_FILE -> $BAO_DEMOS_WRKDIR_IMGS/"
    cp "$BIN_FILE" "$BAO_DEMOS_WRKDIR_IMGS/"
  else
    echo "Error: Expected output not found: $BIN_FILE" >&2
    echo "  Listing $VM_BUILD_DIR:" >&2
    ls -la "$VM_BUILD_DIR/" 2>&1 >&2
    return 1
  fi
}

# Build logic
if "$build_all"; then
  for i in {0..3}; do
    build "$i"
  done
elif [ -n "$VM_INDEX" ]; then
  build "$VM_INDEX"
else
  echo "Error: VM_INDEX is not defined." >&2
  exit 1
fi

echo ""
echo "Finished. Results:"
ls -lh "$BAO_DEMOS_WRKDIR_IMGS"/${VM_NAME_PREFIX}*.bin 2>/dev/null

exit 0
