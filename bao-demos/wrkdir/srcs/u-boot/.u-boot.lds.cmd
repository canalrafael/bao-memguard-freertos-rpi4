cmd_u-boot.lds := /home/main/Documents/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gcc -E -Wp,-MD,./.u-boot.lds.d -D__KERNEL__ -D__UBOOT__ -D__ARM__          -fno-pic  -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9  -mgeneral-regs-only    -fno-common -ffixed-x18 -mbranch-protection=none -pipe -march=armv8-a+crc -D__LINUX_ARM_ARCH__=8  -I./arch/arm/mach-bcm283x/include -Iinclude   -I./arch/arm/include -include ./include/linux/kconfig.h -nostdinc -isystem /home/main/Documents/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/../lib/gcc/aarch64-none-elf/11.2.1/include -ansi -include ./include/u-boot/u-boot.lds.h -DCPUDIR=arch/arm/cpu/armv8  -D__ASSEMBLY__ -x assembler-with-cpp -std=c99 -P -o u-boot.lds arch/arm/cpu/armv8/u-boot.lds

source_u-boot.lds := arch/arm/cpu/armv8/u-boot.lds

deps_u-boot.lds := \
    $(wildcard include/config/armv8/secure/base.h) \
    $(wildcard include/config/armv8/psci.h) \
    $(wildcard include/config/armv8/psci/nr/cpus.h) \
    $(wildcard include/config/linux/kernel/image/header.h) \
  include/linux/kconfig.h \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
    $(wildcard include/config/spl/.h) \
    $(wildcard include/config/tpl/.h) \
    $(wildcard include/config/tools/.h) \
    $(wildcard include/config/tpl/build.h) \
    $(wildcard include/config/vpl/build.h) \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/tools/foo.h) \
    $(wildcard include/config/spl/foo.h) \
    $(wildcard include/config/tpl/foo.h) \
    $(wildcard include/config/vpl/foo.h) \
    $(wildcard include/config/option.h) \
    $(wildcard include/config/acme.h) \
    $(wildcard include/config/spl/acme.h) \
    $(wildcard include/config/tpl/acme.h) \
    $(wildcard include/config/if/enabled/int.h) \
    $(wildcard include/config/int/option.h) \
  include/u-boot/u-boot.lds.h \
  include/config.h \
    $(wildcard include/config/boarddir.h) \
  include/config_uncmd_spl.h \
    $(wildcard include/config/spl/dm.h) \
    $(wildcard include/config/dm/serial.h) \
    $(wildcard include/config/dm/i2c.h) \
    $(wildcard include/config/dm/stdio.h) \
  include/configs/rpi.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/sys/timer/rate.h) \
    $(wildcard include/config/sys/timer/counter.h) \
    $(wildcard include/config/sys/sdram/base.h) \
    $(wildcard include/config/sys/uboot/base.h) \
    $(wildcard include/config/sys/text/base.h) \
    $(wildcard include/config/sys/sdram/size.h) \
    $(wildcard include/config/cmd/dfu.h) \
    $(wildcard include/config/auto/zreladdr.h) \
    $(wildcard include/config/cmd/mmc.h) \
    $(wildcard include/config/cmd/usb.h) \
    $(wildcard include/config/cmd/pxe.h) \
    $(wildcard include/config/cmd/dhcp.h) \
    $(wildcard include/config/extra/env/settings.h) \
  include/linux/sizes.h \
  include/linux/const.h \
  arch/arm/include/asm/arch/timer.h \
  include/config_distro_bootcmd.h \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/cmd/ubifs.h) \
    $(wildcard include/config/efi/loader.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/x86/run/32bit.h) \
    $(wildcard include/config/x86/run/64bit.h) \
    $(wildcard include/config/arch/rv32i.h) \
    $(wildcard include/config/arch/rv64i.h) \
    $(wildcard include/config/cmd/bootefi/bootmgr.h) \
    $(wildcard include/config/sata.h) \
    $(wildcard include/config/nvme.h) \
    $(wildcard include/config/scsi.h) \
    $(wildcard include/config/ide.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/cmd/virtio.h) \
    $(wildcard include/config/x86.h) \
  arch/arm/include/asm/config.h \
    $(wildcard include/config/arch/ls1021a.h) \
    $(wildcard include/config/fsl/layerscape.h) \
  include/linux/kconfig.h \
  include/config_fallbacks.h \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \
    $(wildcard include/config/sys/baudrate/table.h) \
  arch/arm/include/asm/psci.h \
    $(wildcard include/config/armv7/psci/nr/cpus.h) \

u-boot.lds: $(deps_u-boot.lds)

$(deps_u-boot.lds):
