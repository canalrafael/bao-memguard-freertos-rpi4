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
## ✅ Makefile Targets
The `Makefile` provides shortcuts for individual build steps.

| Target      | Description                                                                  |
|:-----------|:-----------------------------------------------------------------------------|
| `make bao` | Compiles the Bao hypervisor by calling `./scripts/util/build_bao.sh`.        |
| `make vms` | Compiles all guest VMs by calling `./scripts/util/build_vm.sh all`.          |
| `make copy`| Copies all binaries to the SD card by calling `./scripts/util/copy_files.sh`.|
| `make clean`| Cleans all build artifacts by calling `./scripts/util/clean.sh`.             |

---
## 📂 Source Code Overview
Here’s a quick guide to the main source code locations:

* **Guest VMs**: The four FreeRTOS virtual machines are located in `bao-demos/wrkdir/srcs/VMs/`. Each `vm_X` directory is a separate application with its entry point at `src/main.c`.

* **Shared Guest Code**: Common code used by all VMs (for benchmarking, utilities, etc.) is in `bao-demos/wrkdir/srcs/guest_common/`.

* **Bao Hypervisor**: The core source code for the Bao hypervisor is located in `bao-demos/wrkdir/srcs/bao/`.

* **System Configuration**: The master blueprint that defines the system layout for Bao (memory maps, peripheral assignments, etc.) is the file: `bao-demos/demos/linux+freertos/configs/rpi4.c`.

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
