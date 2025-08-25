# MGRW - Bao Hypervisor Demo for Raspberry Pi 4
This repository provides a complete environment to compile, deploy, and manage a **Linux + FreeRTOS** multi-guest demo on a **Raspberry Pi 4** using the [Bao Hypervisor](https://github.com/bao-project/bao-hypervisor).

The build system automates the setup of all necessary components, including **U-Boot**, **ARM Trusted Firmware (ATF)**, a **Linux v6.1** kernel, and multiple **FreeRTOS** guest applications.

---
## 🔌 Hardware Requirements
* **Target Board**: A Raspberry Pi 4 Model B.

* **Storage**: A **microSD Card** to hold the binaries, along with a USB SD card reader/adapter to flash it from your PC.

* **Serial Connection Cable**: A **USB to TTL Serial Cable**. This connects from a USB port on your PC to the GPIO pins (TX, RX, GND) on the Raspberry Pi, which is required for serial console access with `minicom`.
    

* **Power Supply**: A reliable **USB-C power supply** (5V/3A) for the Raspberry Pi 4.

---
## 📂 Source Code Overview
Here’s a quick guide to the main source code locations:

* **Guest VMs**: The four FreeRTOS virtual machines are located in `bao-demos/wrkdir/srcs/VMs/`. Each `vm_X` directory is a separate application with its entry point at `src/main.c`.

* **Shared Guest Code**: Common code used by all VMs (for benchmarking, utilities, etc.) is in `bao-demos/wrkdir/srcs/guest_common/`.

* **Bao Hypervisor**: The core source code for the Bao hypervisor is located in `bao-demos/wrkdir/srcs/bao/`.

* **System Configuration**: The master blueprint that defines the system layout for Bao (memory maps, peripheral assignments, etc.) is the file: `bao-demos/demos/linux+freertos/configs/rpi4.c`.

---
## ⚙️ One-Time Configuration
Before your first use, you must configure the SD card paths in the main environment script.

➡️ **Action:** Open the `env.bash` file and modify these two variables to match your system:
* `sd_card_partition`: The device file for your SD card (e.g., `/dev/sdX`).
* `sd_card_path`: The path where the SD card's boot partition is mounted (e.g., `/media/$USER/boot`).

> ⚠️ **Important:** You do **not** need to run `source ./env.bash` yourself. The `make` commands and helper scripts in the project handle this automatically.

---
## 🚀 Core Workflow
The typical workflow involves building the components, copying them to the SD card, and booting the target.

### 1. Build & Deploy (Easy Way)
The `scripts/build` file automates the entire process in the correct order: it builds the VMs, then Bao, and finally copies the files to the SD card.
```bash
./scripts/build
```
---
### (Alternative) Manual Build Steps
If you prefer to run each step individually, you must follow this specific order.

#### 1. Build the Virtual Machines (Build first)
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
---
### 2. Connect to Target
The `minicom` script conveniently copies the required boot command to your clipboard and then opens the serial console.
```bash
./scripts/minicom
```
### 3. Boot the Target in U-Boot
1.  When you see the U-Boot countdown message (`Hit any key to stop autoboot:`), **press any key** to interrupt the process.
2.  This will drop you into the U-Boot command prompt (`U-Boot>`).
3.  From there, **paste** the command from your clipboard and press Enter to load and execute the Bao hypervisor.
```
U-Boot> fatload mmc 0 0x200000 bao.bin; go 0x200000
```
---
## 🔬 Benchmarking
The `benchmarks/` directory contains a powerful suite to automate building, running, and collecting data for a large number of test scenarios. The workflow is split into generating binaries, running the tests, and post-processing the data.

### Directory Structure
* `bin/`: An empty directory where the unique `bao-X.bin` benchmark binaries **will be generated** by the `iterate.sh` script. This directory is ignored by version control.
* `logs/`: Stores raw `.log` output from each test run. It also contains Python scripts to parse and process this data.
* `logs/cleaned_logs/`: A subdirectory created by the processing scripts to hold the parsed `.csv` data files.
* `scripts/`: Holds the main automation scripts (`iterate.sh`, `run_benchmarks.sh`).
* `vms_on_off.txt`: The configuration file where each line defines a unique scenario (e.g., which VMs are active) for a benchmark build.

### Benchmarking Workflow
The following commands should be run from the project's root directory.

#### Step 1: Generate Binaries
**This is a mandatory first step.** You must run this script to generate the unique binaries required for the tests. It iterates through `vms_on_off.txt`, modifies the C source code for each scenario, builds the project, and saves the resulting `bao.bin` as a versioned file in the `benchmarks/bin/` directory.
```bash
./benchmarks/scripts/iterate.sh
```

#### Step 2: Run the Benchmarks
This script executes all the generated binaries on the target board sequentially. It automates the U-Boot process and captures the full serial output for each run into the `benchmarks/logs/` directory.
```bash
./benchmarks/scripts/run_benchmarks.sh
```

#### Step 3: Process the Results
After the tests finish, you can parse the raw log files into a unified CSV dataset using the provided Python scripts.

1.  **Parse Raw Logs**: The `clean.py` script reads each `.log` file, intelligently extracts the CSV-formatted data from the raw serial output, and saves it into a new file inside `benchmarks/logs/cleaned_logs/`.
    ```bash
    cd benchmarks/logs/ && python3 clean.py
    ```

2.  **Remove Invalid Results (Optional)**: If some tests failed to produce data, you can run `delete_invalid_cleaned.py` to automatically remove the empty/invalid CSV files from the `cleaned_logs` directory.
    ```bash
    cd benchmarks/logs/ && python3 delete_invalid_cleaned.py
    ```

3.  **Merge Cleaned Data**: Finally, the `merge.py` script in the `cleaned_logs` directory combines all the individual `.csv` files into a single, comprehensive dataset for analysis.
    ```bash
    cd benchmarks/logs/cleaned_logs/ && python3 merge.py
    ```
    This will produce the final `merged_output.csv` file in the `cleaned_logs` directory.

---
## ✅ Makefile Targets
The `Makefile` provides shortcuts for individual build steps.

| Target      | Description                                                                  |
|:-----------|:-----------------------------------------------------------------------------|
| `make bao` | Compiles the Bao hypervisor by calling `./scripts/util/build_bao.sh`.        |
| `make vms` | Compiles all guest VMs by calling `./scripts/util/build_vm.sh all`.          |
| `make copy`| Copies all binaries to the SD card by calling `./scripts/util/copy_files.sh`.|
| `make clean`| Cleans all build artifacts by calling `./scripts/util/clean.sh`.             |
---
## 📜 Scripts Details
The project's scripts automate complex tasks and provide helpful shortcuts.

### Main Scripts (`scripts/`)
| Name          | Description                                                                                                   |
|:--------------|:--------------------------------------------------------------------------------------------------------------|
| `build`       | **All-in-one script:** Builds all VMs, builds Bao, and copies all files to the SD card.                         |
| `minicom`     | Copies the U-Boot boot command to the clipboard and immediately launches `minicom`.                           |
| `string`      | Copies the U-Boot boot command (`fatload...; go...`) to the clipboard without launching `minicom`.             |

### Editing Shortcuts (`scripts/edit/`)
These scripts use the `$EDITOR` variable to open relevant source files.
| Name     | Description                                                                                          |
|:---------|:-----------------------------------------------------------------------------------------------------|
| `bao`    | Opens the main Bao hypervisor configuration file.                                                    |
| `bench`  | Opens `bench.c`/`.h` for guest benchmark logic.                                                      |
| `budget` | Opens `budget.c`/`.h` for resource budget logic.                                                     |
| `data`   | Opens `data.c`/`.h` for shared data structures.                                                      |
| `misc`   | Opens `misc.c`/`.h` for miscellaneous functions.                                                     |
| `vm`     | Opens the `main.c` of a specific FreeRTOS VM. **Requires a VM index as an argument** (e.g., `./scripts/edit/vm 2`). |

### Utility Scripts (`scripts/util/`)
These are the core build scripts called by the `Makefile` and the main `build` script.
| Name            | Description                                                                  |
|:----------------|:-----------------------------------------------------------------------------|
| `build_bao.sh`  | Compiles the Bao hypervisor source.                                          |
| `build_vm.sh`   | Compiles the Linux and FreeRTOS guests. **Accepts an index (0-3) or `all`.** |
| `copy_files.sh` | Copies all build artifacts to the SD card mount point.                       |
| `clean.sh`      | Removes all generated binaries and intermediate build files.                 |
