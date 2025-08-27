#!/usr/bin/env python3
import re
import sys
from pathlib import Path

if len(sys.argv) != 6:
    print(f"Usage: {sys.argv[0]} <bench_index> <vm0> <vm1> <vm2> <vm3>")
    sys.exit(1)

bench_index = int(sys.argv[1]) - 1
exec_vm = [int(sys.argv[i]) for i in range(2, 6)]

bench_file = Path("bao-demos/wrkdir/srcs/guest_common/inc/benchmarks.h")
reg_file = Path("bao-demos/wrkdir/srcs/guest_common/inc/regulation.h")

if not bench_file.exists():
    sys.exit(f"Error: {bench_file} not found.")
if not reg_file.exists():
    sys.exit(f"Error: {reg_file} not found.")

# --- Update BENCH_ARRAY_INDEX in bench.h ---
bench_text = bench_file.read_text()
bench_text, count_bench = re.subn(
    r"(#define\s+BENCH_ARRAY_INDEX\s+)(?:-?\d+|%d)",
    rf"\g<1>{bench_index}",
    bench_text
)
bench_file.write_text(bench_text)

# --- Update EXEC_VM_x in regulation.h ---
reg_text = reg_file.read_text()
count_exec_total = 0
for i, val in enumerate(exec_vm):
    reg_text, count_exec = re.subn(
        rf"(#define\s+EXEC_VM_{i}\s+)\d+",
        rf"\g<1>{val}",
        reg_text
    )
    count_exec_total += count_exec
reg_file.write_text(reg_text)

print(f"Updated {bench_file}: BENCH_ARRAY_INDEX → {bench_index} ({count_bench} occurrence(s) changed)")
print(f"Updated {reg_file}: EXEC_VM → {exec_vm} ({count_exec_total} occurrence(s) changed)")
