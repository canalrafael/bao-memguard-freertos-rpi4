#!/usr/bin/env bash

current_path=$(realpath .)
bao_folder="bao-demos"
toolchain_folder="arm-gnu-toolchain"

if [ ! -d "$bao_folder" ]; then
  echo "'$bao_folder' not found. Are you in the correct folder?"
  return
fi
if [ ! -d "$toolchain_folder" ]; then
  echo "'$toolchain' not found. Is the project structured correctly?"
  return
fi

#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

### [https://github.com/bao-project/bao-demos/tree/master]
# (0) https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
export PYTHONPATH=/usr
#export CROSS_COMPILE=/home/$USER/code/bao/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
#export CROSS_COMPILE=$PWD/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
export CROSS_COMPILE="$current_path"/arm-gnu-toolchain/bin/aarch64-none-elf-

# (1)
export PLATFORM=rpi4
export DEMO=linux+freertos
export ARCH=aarch64


export BAO_DEMOS="$current_path"/bao-demos
export BAO_DEMOS_WRKDIR=$BAO_DEMOS/wrkdir
export BAO_DEMOS_WRKDIR_SRC=$BAO_DEMOS_WRKDIR/srcs
export BAO_DEMOS_WRKDIR_BIN=$BAO_DEMOS_WRKDIR/bin
export BAO_DEMOS_WRKDIR_PLAT=$BAO_DEMOS_WRKDIR/imgs/$PLATFORM
export BAO_DEMOS_WRKDIR_IMGS=$BAO_DEMOS_WRKDIR_PLAT/$DEMO

# https://github.com/bao-project/bao-demos/blob/master/demos/linux+freertos/README.md
# export FVPR_VM_IMAGES="$BAO_DEMOS_WRKDIR_IMGS/freertos.bin@0x10000000 \
#    $BAO_DEMOS_WRKDIR_IMGS/linux.bin@0x20000000"

### [FreeRTOS] + [Linux]
# https://github.com/bao-project/bao-demos/blob/master/guests/freertos/README.md
if [ -z "$VM_INDEX" ]; then
  echo "Error: The variable VM_INDEX is not defined or is empty." >&2
fi

export BAO_DEMOS_FREERTOS=$BAO_DEMOS_WRKDIR_SRC/VMs/vm_$VM_INDEX/
export FREERTOS_PARAMS="STD_ADDR_SPACE=y"

####

export BAO_DEMOS_LINUX=$BAO_DEMOS/guests/linux
export BAO_DEMOS_LINUX_REPO=https://github.com/torvalds/linux.git
export BAO_DEMOS_LINUX_VERSION=v6.1
export BAO_DEMOS_LINUX_SRC=$BAO_DEMOS_WRKDIR_SRC/linux-$BAO_DEMOS_LINUX_VERSION

export BAO_DEMOS_LINUX_CFG_FRAG=$(ls $BAO_DEMOS_LINUX/configs/base.config\
    $BAO_DEMOS_LINUX/configs/$ARCH.config\
    $BAO_DEMOS_LINUX/configs/$PLATFORM.config 2> /dev/null)

export BAO_DEMOS_BUILDROOT=$BAO_DEMOS_WRKDIR_SRC/\
buildroot-$ARCH-$BAO_DEMOS_LINUX_VERSION
export BAO_DEMOS_BUILDROOT_DEFCFG=$BAO_DEMOS_LINUX/buildroot/$ARCH.config
export LINUX_OVERRIDE_SRCDIR=$BAO_DEMOS_LINUX_SRC

export BAO_DEMO_LINUX_VM=linux

#####
# (B.4)
export BAO_DEMOS_BAO=$BAO_DEMOS_WRKDIR_SRC/bao

# (B.5)
export BAO_DEMOS_FW=$BAO_DEMOS_WRKDIR_PLAT/firmware
export BAO_DEMOS_UBOOT=$BAO_DEMOS_WRKDIR_SRC/u-boot
export BAO_DEMOS_ATF=$BAO_DEMOS_WRKDIR_SRC/arm-trusted-firmware

# (Prepare SDCard)
export BAO_DEMOS_SDCARD_DEV=/dev/sda
export BAO_DEMOS_SDCARD=/media/$USER/boot

#####################################

export VM=$BAO_DEMOS/wrkdir/srcs/VMs/vm_
export BAO_CORE=$BAO_DEMOS_WRKDIR_SRC/bao/src/core
