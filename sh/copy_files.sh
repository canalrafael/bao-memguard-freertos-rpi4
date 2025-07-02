#!/usr/bin/env bash

VM_INDEX=9 source env.bash

## # u-boot
## cd $BAO_DEMOS_UBOOT
## make rpi_4_defconfig
## make -j $(nproc)
## cp $BAO_DEMOS_UBOOT/u-boot.bin $BAO_DEMOS_WRKDIR_PLAT

## # tf-a
## cd $BAO_DEMOS_ATF
## make PLAT=rpi4
## cp $BAO_DEMOS_ATF/build/rpi4/release/bl31.bin $BAO_DEMOS_WRKDIR_PLAT

## # sd-card
## if [ ! -d "$BAO_DEMOS_SDCARD" ]; then
##   echo "[ERROR] Folder '$BAO_DEMOS_SDCARD' does not exist. Did you mount the SD-Card?"
##   exit 1
## fi
## #cp -vrf $BAO_DEMOS_WRKDIR_PLAT/firmware/boot/* $BAO_DEMOS_SDCARD
## #cp -v $BAO_DEMOS/platforms/rpi4/config.txt $BAO_DEMOS_SDCARD
## #cp -v $BAO_DEMOS_WRKDIR_PLAT/bl31.bin $BAO_DEMOS_SDCARD
## #cp -v $BAO_DEMOS_WRKDIR_PLAT/u-boot.bin $BAO_DEMOS_SDCARD
rm -rf $BAO_DEMOS_SDCARD/**
cp -v $BAO_DEMOS/../arquivos_SD/** $BAO_DEMOS_SDCARD
cp -v $BAO_DEMOS_WRKDIR_IMGS/bao.bin $BAO_DEMOS_SDCARD
# umount $BAO_DEMOS_SDCARD

echo Finished.;
