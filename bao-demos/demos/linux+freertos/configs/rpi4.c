#include "../../../../srcs/guest_common/inc/regulation.h"
#include <config.h>

VM_IMAGE(vm_0_img, XSTR(BAO_DEMOS_WRKDIR_IMGS/vm_0.bin));
VM_IMAGE(vm_1_img, XSTR(BAO_DEMOS_WRKDIR_IMGS/vm_1.bin));
VM_IMAGE(vm_2_img, XSTR(BAO_DEMOS_WRKDIR_IMGS/vm_2.bin));
VM_IMAGE(vm_3_img, XSTR(BAO_DEMOS_WRKDIR_IMGS/vm_3.bin));

// hey there

struct config config =
    {
        CONFIG_HEADER

            .shmemlist_size = 1,
        .shmemlist = (struct shmem[]){[0] =
                                          {
                                              .size = 0x00010000,
                                            // .size = 0x00400000,  //para suportar mais dados
                                          }},

        .vmlist_size = RUNNING_VMs,
        .vmlist =
            {

#if EXEC_VM_0
                // -------------------
                // CORE 0 - BEGINNING
                // -------------------
                {
                    .image = {.base_addr = 0x0,

                              .load_addr = VM_IMAGE_OFFSET(vm_0_img),
                              .size = VM_IMAGE_SIZE(vm_0_img)},

                    .entry = 0x0,
#if CACHE_COLORING
                    .colors = 0b1,
#else
                    .colors = 0b0,
#endif
                    .cpu_affinity = 0b1,

                    .platform = {.cpu_num = 1,

                                 .region_num = 1,
                                 .regions =
                                     (struct vm_mem_region[]){
                                         {.base = 0x0, .size = 0x8000000}},

                                 .ipc_num = 1,
                                 .ipcs = (struct ipc[]){{.base = 0x70000000,
                                                         .size = 0x00010000,
                                                         .shmem_id = 0,
                                                         .interrupt_num = 1,
                                                         .interrupts =
                                                             (irqid_t[]){52}}},

                                 .dev_num = 2,
                                 .devs =
                                     (struct vm_dev_region[]){
                                          {/* UART1 - mini-UART (GPIO 14/15) */
                                          .pa = 0xfe215000,
                                          .va = 0xff000000,
                                          .size = 0x1000,
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){125}},
                                         {/* Arch timer interrupt */
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){27}},
                                     },
                                 .arch = {.gic =
                                              {
                                                  .gicd_addr = 0xF9010000,
                                                  .gicc_addr = 0xF9020000,
                                              }}},
                },

// ------------
// CORE 0 - END
// ------------
#endif

#if EXEC_VM_1
                // -------------------
                // CORE 1 - BEGINNING
                // -------------------
                {
                    .image = {.base_addr = 0x0,

                              .load_addr = VM_IMAGE_OFFSET(vm_1_img),
                              .size = VM_IMAGE_SIZE(vm_1_img)},

                    .entry = 0x0,
#if CACHE_COLORING
                    .colors = 0b10,
#else
                    .colors = 0b0,
#endif
                    .cpu_affinity = 0b10,

                    .platform = {.cpu_num = 1,

                                 .region_num = 1,
                                 .regions =
                                     (struct vm_mem_region[]){
                                         {.base = 0x0, .size = 0x8000000}},

                                 .ipc_num = 1,
                                 .ipcs = (struct ipc[]){{.base = 0x70000000,
                                                         .size = 0x00010000,
                                                         .shmem_id = 0,
                                                         .interrupt_num = 1,
                                                         .interrupts =
                                                             (irqid_t[]){52}}},

                                 .dev_num = 2,
                                 .devs =
                                     (struct vm_dev_region[]){
                                         {/* UART4 - PL011 para Linux (GPIO 8/9) */
                                          .pa = 0xfe201800,
                                          .va = 0xfe201800,
                                          .size = 0x1000,
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){154}},
                                         {/* Arch timer interrupt */
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){27}},
                                     },
                                 .arch = {.gic =
                                              {
                                                  .gicd_addr = 0xF9010000,
                                                  .gicc_addr = 0xF9020000,
                                              }}},
                },
// ------------
// CORE 1 - END
// ------------
#endif

#if EXEC_VM_2
                // -------------------
                // CORE 2 - BEGINNING
                // -------------------
                {
                    .image = {.base_addr = 0x0,

                              .load_addr = VM_IMAGE_OFFSET(vm_2_img),
                              .size = VM_IMAGE_SIZE(vm_2_img)},

                    .entry = 0x0,
#if CACHE_COLORING
                    .colors = 0b100,
#else
                    .colors = 0b0,
#endif
                    .cpu_affinity = 0b100,

                    .platform = {.cpu_num = 1,

                                 .region_num = 1,
                                 .regions =
                                     (struct vm_mem_region[]){
                                         {.base = 0x0, .size = 0x8000000}},

                                 .ipc_num = 1,
                                 .ipcs = (struct ipc[]){{.base = 0x70000000,
                                                         .size = 0x00010000,
                                                         .shmem_id = 0,
                                                         .interrupt_num = 1,
                                                         .interrupts =
                                                             (irqid_t[]){52}}},

                                 .dev_num = 2,
                                 .devs =
                                     (struct vm_dev_region[]){
                                         {/* UART1 */
                                          .pa = 0xfe215000,
                                          .va = 0xff000000,
                                          .size = 0x1000,
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){127}},
                                         {/* Arch timer interrupt */
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){27}},
                                     },
                                 .arch = {.gic =
                                              {
                                                  .gicd_addr = 0xF9010000,
                                                  .gicc_addr = 0xF9020000,
                                              }}},
                },
// ------------
// CORE 2 - END
// ------------
#endif

#if EXEC_VM_3
                // -------------------
                // CORE 3 - BEGINNING
                // -------------------
                {
                    .image = {.base_addr = 0x0,

                              .load_addr = VM_IMAGE_OFFSET(vm_3_img),
                              .size = VM_IMAGE_SIZE(vm_3_img)},

                    .entry = 0x0,
#if CACHE_COLORING
                    .colors = 0b1000,
#else
                    .colors = 0b0,
#endif
                    .cpu_affinity = 0b1000,

                    .platform = {.cpu_num = 1,

                                 .region_num = 1,
                                 .regions =
                                     (struct vm_mem_region[]){
                                         {.base = 0x0, .size = 0x8000000}},

                                 .ipc_num = 1,
                                 .ipcs = (struct ipc[]){{.base = 0x70000000,
                                                         .size = 0x00010000,
                                                         .shmem_id = 0,
                                                         .interrupt_num = 1,
                                                         .interrupts =
                                                             (irqid_t[]){52}}},

                                 .dev_num = 2,
                                 .devs =
                                     (struct vm_dev_region[]){
                                         {/* UART1 */
                                          .pa = 0xfe215000,
                                          .va = 0xff000000,
                                          .size = 0x1000,
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){128}},
                                         {/* Arch timer interrupt */
                                          .interrupt_num = 1,
                                          .interrupts = (irqid_t[]){27}},
                                     },
                                 .arch = {.gic =
                                              {
                                                  .gicd_addr = 0xF9010000,
                                                  .gicc_addr = 0xF9020000,
                                              }}},
                },
// ------------
// CORE 3 - END
// ------------
#endif

            },
};
