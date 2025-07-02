cmd_lib/efi_loader/efi_crt0.o := /home/main/Documents/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gcc -Wp,-MD,lib/efi_loader/.efi_crt0.o.d -nostdinc -isystem /home/main/Documents/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/../lib/gcc/aarch64-none-elf/11.2.1/include -Iinclude   -I./arch/arm/include -include ./include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -D__ASSEMBLY__ -fno-PIE -g -DHOST_ARCH="0xaa64" -I. -D__ARM__ -fno-pic -mstrict-align -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -mgeneral-regs-only -fno-common -ffixed-x18 -mbranch-protection=none -pipe -march=armv8-a+crc -D__LINUX_ARM_ARCH__=8 -I./arch/arm/mach-bcm283x/include   -c -o lib/efi_loader/efi_crt0.o arch/arm/lib/crt0_aarch64_efi.S

source_lib/efi_loader/efi_crt0.o := arch/arm/lib/crt0_aarch64_efi.S

deps_lib/efi_loader/efi_crt0.o := \
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
  include/asm-generic/pe.h \

lib/efi_loader/efi_crt0.o: $(deps_lib/efi_loader/efi_crt0.o)

$(deps_lib/efi_loader/efi_crt0.o):
