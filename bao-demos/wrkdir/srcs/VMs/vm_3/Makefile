NAME:=vm_3
ROOT_DIR:=$(realpath .)

.DEFAULT_GOAL := all

wrkdir_demo_imgs := $(ROOT_DIR)/build/$(PLATFORM)
TARGET := $(wrkdir_demo_imgs)/$(NAME).bin

wrkdir_src := $(ROOT_DIR)
bao_demos ?= $(BAO_DEMOS)

include $(BAO_DEMOS)/guests/linux/make.mk

BUILDROOT_DIR := $(buildroot_src)
CC_ARM := $(BUILDROOT_DIR)/output/host/bin/aarch64-buildroot-linux-uclibc-gcc
CUSTOM_OVERLAY := $(wrkdir_src)/custom_overlay
OVERLAY_DIR := $(CUSTOM_OVERLAY)/root

SPECTRE_SRC := $(wrkdir_src)/attacks/spectre/src/spectre_attack.c
ARMAGEDDON_SRC := $(wrkdir_src)/attacks/armageddon/src/armageddon.c
MELTDOWN_SRC := $(wrkdir_src)/attacks/meltdown/src/meltdown.c
ZOMBIELOAD_SRC := $(wrkdir_src)/attacks/zombieload/src/zombieload.c

CUSTOM_APPS := $(OVERLAY_DIR)/spectre \
               $(OVERLAY_DIR)/armageddon \
               $(OVERLAY_DIR)/meltdown \
               $(OVERLAY_DIR)/zombieload

# --- Regras de compilação dos ataques ---

$(OVERLAY_DIR)/spectre: $(SPECTRE_SRC)
	@echo ">> Compilando Ataque Spectre..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/armageddon: $(ARMAGEDDON_SRC)
	@echo ">> Compilando Ataque Armageddon..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/meltdown: $(MELTDOWN_SRC)
	@echo ">> Compilando Ataque Meltdown..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/zombieload: $(ZOMBIELOAD_SRC)
	@echo ">> Compilando Ataque ZombieLoad..."
	$(CC_ARM) -O2 $< -o $@

# --- Regra do RootFS ---

$(BUILDROOT_DIR)/output/images/rootfs.cpio: $(CUSTOM_APPS)
	@echo ">> Repacotando o sistema de arquivos do Linux com os ataques..."
	$(MAKE) -C $(BUILDROOT_DIR) BR2_ROOTFS_OVERLAY="$(CUSTOM_OVERLAY)"

all: $(CUSTOM_APPS) $(BUILDROOT_DIR)/output/images/rootfs.cpio $(TARGET)

VM3_DTS := $(BAO_DEMOS)/demos/linux+freertos/devicetrees/$(PLATFORM)/linux.dts

$(eval $(call build-linux, $(TARGET), $(VM3_DTS)))

clean:
	rm -f $(TARGET)
	rm -f $(wrkdir_demo_imgs)/*.dtb