# Prepare enviroment for baremetal-runtime
NAME:=vm_3
ROOT_DIR:=$(realpath .)
BUILD_DIR:=$(ROOT_DIR)/build/$(PLATFORM)

# Setup baremetal-runtime build
bmrt_dir:=$(ROOT_DIR)/src/baremetal-runtime
include $(bmrt_dir)/setup.mk

# Include all freertos sources
INC_GUEST_COMMON=$(BAO_DEMOS_WRKDIR_SRC)/guest_common/inc
SRC_GUEST_COMMON=$(BAO_DEMOS_WRKDIR_SRC)/guest_common/src

# Bandwidth
INC_BANDWIDTH=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/bandwidth/inc
SRC_BANDWIDTH=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/bandwidth/src

# DISPARITY
INC_DISPARITY=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/disparity/inc
SRC_DISPARITY=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/disparity/src

# MSER
INC_MSER=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/mser/inc
SRC_MSER=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/mser/src

# FFT
INC_FFT=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/fft/inc
SRC_FFT=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/fft/src

# Sorting
INC_SORTING=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/sorting/inc
SRC_SORTING=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/sorting/src

# QSort
INC_QSORT=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/qsort/inc
SRC_QSORT=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/qsort/src

# Dijkstra
INC_DIJKSTRA=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/dijkstra/inc
SRC_DIJKSTRA=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/dijkstra/src

# SHA
INC_SHA=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/sha/inc
SRC_SHA=$(BAO_DEMOS_WRKDIR_SRC)/benchmarks/sha/src


# Main freertos application sources
app_src_dir:=$(ROOT_DIR)/src
include $(app_src_dir)/sources.mk
C_SRC+=$(addprefix $(app_src_dir)/, $(src_c_srcs))

# OTHERS SRCS
C_SRC+=$(wildcard $(SRC_GUEST_COMMON)/*.c)
C_SRC+=$(wildcard $(SRC_DISPARITY)/*.c)
C_SRC+=$(wildcard $(SRC_MSER)/*.c)
C_SRC+=$(wildcard $(SRC_BANDWIDTH)/*.c)
C_SRC+=$(wildcard $(SRC_FFT)/*.c)
C_SRC+=$(wildcard $(SRC_SORTING)/*.c)
C_SRC+=$(wildcard $(SRC_QSORT)/*.c)
C_SRC+=$(wildcard $(SRC_DIJKSTRA)/*.c)
C_SRC+=$(wildcard $(SRC_SHA)/*.c)

# Freertos kernel sources
freertos_dir:=$(ROOT_DIR)/src/freertos
SRC_DIRS+=$(freertos_dir) $(freertos_memmng_dir)
INC_DIRS+=$(freertos_dir)/include
C_SRC+=$(wildcard $(freertos_dir)/*.c)
freertos_memmng_dir:=$(freertos_dir)/portable/MemMang
C_SRC+=$(freertos_memmng_dir)/heap_4.c

# Freertos port arch-specific
arch_dir=$(ROOT_DIR)/src/arch/$(ARCH)
SRC_DIRS+=$(arch_dir)
INC_DIRS+=$(arch_dir)/inc
-include $(arch_dir)/arch.mk
-include $(arch_dir)/sources.mk
C_SRC+=$(addprefix $(arch_dir)/, $(arch_c_srcs))
ASM_SRC+=$(addprefix $(arch_dir)/, $(arch_s_srcs))

# OTHERS INCLUDES
INC_DIRS+=$(INC_GUEST_COMMON)
INC_DIRS+=$(INC_DISPARITY)
INC_DIRS+=$(INC_MSER)
INC_DIRS+=$(INC_BANDWIDTH)
INC_DIRS+=$(INC_FFT)
INC_DIRS+=$(INC_SORTING)
INC_DIRS+=$(INC_QSORT)
INC_DIRS+=$(INC_DIJKSTRA)
INC_DIRS+=$(INC_SHA)

# Include the final baremetal-runtime build
include $(bmrt_dir)/build.mk
