# Bao Hypervisor Demo on Raspberry Pi 4

This repository provides a fully automated environment to build and run a **Linux + FreeRTOS** multi-guest demo on a **Raspberry Pi 4** using the [Bao-Hypervisor](https://github.com/bao-project/bao-hypervisor).

---

## 📋 Table of Contents
- [Prerequisites](#-prerequisites)
- [Quick Start](#-quick-start)
- [Detailed Workflow](#️-detailed-workflow)
- [Advanced: Automated Benchmarking](#-advanced-automated-benchmarking)
- [Source Code Overview](#-source-code-overview)
- [Reference](#reference)

---

## 🔧 Prerequisites

Before you begin, make sure you have the following hardware and have completed the one-time configuration step.

### 1. Hardware Requirements
- **Target Board**: Raspberry Pi 4 Model B.
- **Storage**: A microSD Card and a USB reader to flash it.
- **Serial Connection**: A USB to TTL Serial Cable for console access.
- **Power Supply**: A reliable USB-C power supply (5V/3A).

### 2. One-Time Configuration
You must tell the build system where your SD card is.

➡️ **Action**: Open the `env.bash` file and set the following variables:

```bash
# env.bash

# The device file for your SD card (e.g., /dev/sdc).
# Use `lsblk` to find the correct device.
sd_card_partition="/dev/sdX"

# The path where the SD card's boot partition is mounted (e.g., /media/$USER/boot).
sd_card_path="/media/your_user/boot"
```
> ⚠️ **Important:** You do **not** need to `source ./env.bash` yourself. The `make` commands and scripts handle this automatically.

---

## 🚀 Quick Start

This is the fastest way to get the demo running.

### Step 1: Build & Deploy
Run the main `build` script. This compiles all VMs and the hypervisor, then copies the binaries to your configured SD card.

```bash
./scripts/build
```

### Step 2: Connect & Boot
1.  Run the `minicom` script. It copies the required U-Boot command to your clipboard and opens the serial console.
    ```bash
    ./scripts/minicom
    ```
2.  Power on your Raspberry Pi 4.
3.  When you see the `Hit any key to stop autoboot:` message in `minicom`, press any key to enter the U-Boot prompt (`U-Boot>`).
4.  Paste the command from your clipboard and press **Enter**.

    ```
    U-Boot> fatload mmc 0 0x200000 bao.bin; go 0x200000
    ```
The Bao hypervisor will now boot, followed by the Linux and FreeRTOS guests.

---

## ⚙️ Detailed Workflow

If you prefer to run each step manually, follow this sequence.

#### 1. Build Virtual Machines
```bash
# Build all VMs
make vms

# Or, to build a single VM (e.g., VM 2)
./scripts/util/build_vm.sh 2
```

#### 2. Build the Bao Hypervisor
```bash
make bao
```

#### 3. Deploy to SD Card
```bash
make copy
```
After this, follow **Step 2** from the [Quick Start](#-quick-start) to connect and boot the device.

---

## 📂 Source Code Overview
- **Guest VMs**: `bao-demos/wrkdir/srcs/VMs/` (Each `vm_X` is a separate FreeRTOS instance).
- **Shared Guest Code**: `bao-demos/wrkdir/srcs/guest_common/`.
- **Bao Hypervisor**: `bao-demos/wrkdir/srcs/bao/`.
- **System Configuration**: `bao-demos/demos/linux+freertos/configs/rpi4.c`.

---

<details>
<summary>
<h3>🔬 Advanced: Automated Benchmarking</h3>
</summary>

The `benchmarks/` directory contains a powerful suite to automate building, running, and collecting data.

#### Directory Structure
* `bin/`: Target directory where unique `bao-X.bin` binaries are generated.
* `logs/`: Stores raw `.log` output from each test run and Python processing scripts.
* `logs/cleaned_logs/`: Subdirectory for parsed `.csv` data files.
* `scripts/`: Holds main automation scripts (`iterate.sh`, `run_benchmarks.sh`).
* `vms_on_off.txt`: The configuration file defining unique test scenarios.

#### Benchmarking Workflow

**Step 1: Generate Binaries (Mandatory)**
This script iterates through `vms_on_off.txt`, modifies the source for each scenario, builds the project, and saves a unique `bao-X.bin` file in `benchmarks/bin/`.

```bash
./benchmarks/scripts/iterate.sh
```

**Step 2: Run Benchmarks**
This script automates flashing and running each generated binary on the target, capturing the serial output to a log file.

```bash
./benchmarks/scripts/run_benchmarks.sh
```

**Step 3: Process Results**
Use the provided Python scripts to parse the raw logs into a final, merged CSV dataset.

```bash
# 1. Go to the logs directory
cd benchmarks/logs/

# 2. Parse raw logs into individual CSVs
python3 clean.py

# 3. (Optional) Remove any empty/invalid results
python3 delete_invalid_cleaned.py

# 4. Go to the cleaned logs directory
cd cleaned_logs/

# 5. Merge all CSVs into a single dataset
python3 merge.py
```
This produces the final `merged_output.csv` for analysis.

</details>

---

<details id="reference">
<summary>
<h3>📘 Reference: Scripts & Makefile Targets</h3>
</summary>

#### Makefile Targets
| Target     | Description                                                                 |
|:-----------|:----------------------------------------------------------------------------|
| `make bao` | Compiles the Bao hypervisor.                                                |
| `make vms` | Compiles all guest VMs.                                                     |
| `make copy`| Copies all binaries to the SD card.                                         |
| `make clean`| Cleans all build artifacts.                                                 |

#### Scripts
A full list of helper scripts for building, editing, and running.

| Path                  | Name        | Description                                                          |
|:----------------------|:------------|:---------------------------------------------------------------------|
| `scripts/`            | `build`     | **All-in-one:** Builds VMs, Bao, and copies files to the SD card.      |
| `scripts/`            | `minicom`   | Copies boot command and launches `minicom`.                            |
| `scripts/`            | `string`    | Copies boot command without launching `minicom`.                       |
| `scripts/edit/`       | `vm`, `bao`...  | Shortcuts to open key source files in your `$EDITOR`.                |
| `scripts/util/`       | `build_`... | Core build scripts called by the Makefile and other scripts.           |

</details>

<!-- > README.md (26/08/25), [Author](https://github.com/nynepebbles) -->
