NAME:=vm_1
ROOT_DIR:=$(realpath .)

.DEFAULT_GOAL := all

wrkdir_demo_imgs := $(ROOT_DIR)/build/$(PLATFORM)
TARGET := $(wrkdir_demo_imgs)/$(NAME).bin

# Apontando para a pasta compartilhada onde o Buildroot e os seus códigos estão
wrkdir_src := $(realpath $(ROOT_DIR)/../shared_src)
bao_demos ?= $(BAO_DEMOS)

include $(BAO_DEMOS)/guests/linux/make.mk

BUILDROOT_DIR := $(wrkdir_src)/buildroot-aarch64-v6.1
CC_ARM := $(BUILDROOT_DIR)/output/host/bin/aarch64-buildroot-linux-uclibc-gcc
OVERLAY_DIR := $(wrkdir_src)/custom_overlay/root

# Caminhos exatos para os arquivos de ataque
SPECTRE_SRC := $(wrkdir_src)/attacks/spectre/src/spectre_attack.c
ARMAGEDDON_SRC := $(wrkdir_src)/attacks/armageddon/src/armageddon.c

# Caminhos exatos para o IPC
IPC_SRC := $(wrkdir_src)/ipc_receiver.c

# Lista de todos os arquivos .c que compõem o executável do Benchmark
BENCH_SOURCES := $(wrkdir_src)/main_benchmark.c \
                 $(wrkdir_src)/benchmarks/wrappers/src/benchmarks_wrappers.c \
                 $(wrkdir_src)/benchmarks/bandwidth/src/bandwidth.c \
                 $(wrkdir_src)/benchmarks/dijkstra/src/dijkstra.c \
                 $(wildcard $(wrkdir_src)/benchmarks/disparity/src/*.c) \
                 $(wildcard $(wrkdir_src)/benchmarks/fft/src/*.c) \
                 $(wrkdir_src)/benchmarks/qsort/src/qsort.c \
                 $(wrkdir_src)/benchmarks/sha/src/sha.c \
                 $(wrkdir_src)/benchmarks/sorting/src/sorting.c

# Variável contendo todos os binários finais que precisamos gerar no RootFS
CUSTOM_APPS := $(OVERLAY_DIR)/ipc_receiver \
               $(OVERLAY_DIR)/spectre \
               $(OVERLAY_DIR)/armageddon \
               $(OVERLAY_DIR)/benchmark \
               $(OVERLAY_DIR)/trigger_print

# --- Regras individuais de compilação ---

$(OVERLAY_DIR)/ipc_receiver: $(IPC_SRC)
	@echo ">> Compilando Receptor IPC..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/spectre: $(SPECTRE_SRC)
	@echo ">> Compilando Ataque Spectre..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/armageddon: $(ARMAGEDDON_SRC)
	@echo ">> Compilando Ataque Armageddon..."
	$(CC_ARM) -O2 $< -o $@

$(OVERLAY_DIR)/benchmark: $(BENCH_SOURCES)
	@echo ">> Compilando pacote de Benchmarks..."
	$(CC_ARM) -O2 $^ -o $@ -lm

TRIGGER_PRINT_SRC := $(wrkdir_src)/trigger_print.c

$(OVERLAY_DIR)/trigger_print: $(TRIGGER_PRINT_SRC)
	@echo ">> Compilando trigger print..."
	$(CC_ARM) -O2 $< -o $@

# --- Regra do RootFS ---
$(BUILDROOT_DIR)/output/images/rootfs.cpio: $(CUSTOM_APPS)
	@echo ">> Repacotando o sistema de arquivos do Linux com os novos executaveis..."
	$(MAKE) -C $(BUILDROOT_DIR)

all: $(CUSTOM_APPS) $(BUILDROOT_DIR)/output/images/rootfs.cpio $(TARGET)

VM1_DTS := $(BAO_DEMOS)/demos/linux+freertos/devicetrees/$(PLATFORM)/linux_vm_1.dts

$(eval $(call build-linux, $(TARGET), $(VM1_DTS)))

clean:
	rm -f $(TARGET)
	rm -f $(wrkdir_demo_imgs)/*.dtb