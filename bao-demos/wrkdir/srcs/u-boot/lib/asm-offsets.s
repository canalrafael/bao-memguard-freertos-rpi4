	.arch armv8-a+crc
	.file	"asm-offsets.c"
// GNU C11 (GNU Toolchain for the Arm Architecture 11.2-2022.02 (arm-11.14)) version 11.2.1 20220111 (aarch64-none-elf)
//	compiled by GNU C version 4.8.5 20150623 (Red Hat 4.8.5-44), GMP version 6.2.1, MPFR version 3.1.6, MPC version 1.0.3, isl version isl-0.15-1-g835ea3a-GMP

// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed: -mstrict-align -mgeneral-regs-only -mbranch-protection=none -march=armv8-a+crc -mlittle-endian -mabi=lp64 -g -Os -std=gnu11 -fno-builtin -ffreestanding -fshort-wchar -fno-strict-aliasing -fno-stack-protector -fno-delete-null-pointer-checks -fstack-usage -fno-pic -ffunction-sections -fdata-sections -ffixed-r9 -fno-common -ffixed-x18
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/main/Documents/board/BAO_hypervisor/Linux_FreeRTOS/bao-demos/bao-demos/wrkdir/srcs/u-boot" "lib/asm-offsets.c"
	.section	.text.startup.main,"ax",@progbits
	.align	2
	.global	main
	.type	main, %function
main:
.LFB135:
	.file 1 "lib/asm-offsets.c"
	.loc 1 21 1 view -0
	.cfi_startproc
	.loc 1 23 2 view .LVU1
	// Start of user assembly
// 23 "lib/asm-offsets.c" 1
	
.ascii "->GENERATED_GBL_DATA_SIZE 464 (sizeof(struct global_data) + 15) & ~15"	//
// 0 "" 2
	.loc 1 26 2 view .LVU2
// 26 "lib/asm-offsets.c" 1
	
.ascii "->GENERATED_BD_INFO_SIZE 192 (sizeof(struct bd_info) + 15) & ~15"	//
// 0 "" 2
	.loc 1 29 2 view .LVU3
// 29 "lib/asm-offsets.c" 1
	
.ascii "->GD_SIZE 464 sizeof(struct global_data)"	//
// 0 "" 2
	.loc 1 31 2 view .LVU4
// 31 "lib/asm-offsets.c" 1
	
.ascii "->GD_BD 0 offsetof(struct global_data, bd)"	//
// 0 "" 2
	.loc 1 33 2 view .LVU5
// 33 "lib/asm-offsets.c" 1
	
.ascii "->GD_FLAGS 8 offsetof(struct global_data, flags)"	//
// 0 "" 2
	.loc 1 36 2 view .LVU6
// 36 "lib/asm-offsets.c" 1
	
.ascii "->GD_MALLOC_BASE 296 offsetof(struct global_data, malloc_base)"	//
// 0 "" 2
	.loc 1 39 2 view .LVU7
// 39 "lib/asm-offsets.c" 1
	
.ascii "->GD_RELOCADDR 120 offsetof(struct global_data, relocaddr)"	//
// 0 "" 2
	.loc 1 41 2 view .LVU8
// 41 "lib/asm-offsets.c" 1
	
.ascii "->GD_RELOC_OFF 160 offsetof(struct global_data, reloc_off)"	//
// 0 "" 2
	.loc 1 43 2 view .LVU9
// 43 "lib/asm-offsets.c" 1
	
.ascii "->GD_START_ADDR_SP 152 offsetof(struct global_data, start_addr_sp)"	//
// 0 "" 2
	.loc 1 45 2 view .LVU10
// 45 "lib/asm-offsets.c" 1
	
.ascii "->GD_NEW_GD 168 offsetof(struct global_data, new_gd)"	//
// 0 "" 2
	.loc 1 47 2 view .LVU11
// 47 "lib/asm-offsets.c" 1
	
.ascii "->GD_ENV_ADDR 72 offsetof(struct global_data, env_addr)"	//
// 0 "" 2
	.loc 1 49 2 view .LVU12
// lib/asm-offsets.c:50: }
	.loc 1 50 1 is_stmt 0 view .LVU13
	// End of user assembly
	mov	w0, 0	//,
	ret	
	.cfi_endproc
.LFE135:
	.size	main, .-main
	.text
.Letext0:
	.file 2 "include/errno.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0xc7
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0x3
	.4byte	.LASF14
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.4byte	.LLRL0
	.8byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x2
	.4byte	0x38
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF8
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF9
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF10
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x5
	.4byte	0x3f
	.4byte	0x8c
	.uleb128 0x6
	.4byte	0x2a
	.byte	0
	.byte	0
	.uleb128 0x2
	.4byte	0x7c
	.uleb128 0x7
	.4byte	.LASF15
	.byte	0x2
	.byte	0x1f
	.byte	0x13
	.4byte	0x8c
	.string	""
	.uleb128 0x1
	.byte	0x10
	.byte	0x4
	.4byte	.LASF12
	.uleb128 0x1
	.byte	0x1
	.byte	0x2
	.4byte	.LASF13
	.uleb128 0x8
	.4byte	.LASF16
	.byte	0x1
	.byte	0x14
	.byte	0x5
	.4byte	0x31
	.8byte	.LFB135
	.8byte	.LFE135-.LFB135
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.LFB135
	.8byte	.LFE135-.LFB135
	.8byte	0
	.8byte	0
	.section	.debug_rnglists,"",@progbits
.Ldebug_ranges0:
	.4byte	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.2byte	0x5
	.byte	0x8
	.byte	0
	.4byte	0
.LLRL0:
	.byte	0x7
	.8byte	.LFB135
	.uleb128 .LFE135-.LFB135
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF4:
	.string	"unsigned int"
.LASF16:
	.string	"main"
.LASF2:
	.string	"long unsigned int"
.LASF11:
	.string	"long long unsigned int"
.LASF9:
	.string	"unsigned char"
.LASF3:
	.string	"char"
.LASF6:
	.string	"long int"
.LASF15:
	.string	"error_message"
.LASF13:
	.string	"_Bool"
.LASF7:
	.string	"long long int"
.LASF5:
	.string	"short unsigned int"
.LASF8:
	.string	"signed char"
.LASF14:
	.ascii	"GNU C11 11.2.1 20220111 -mstrict-align -mgeneral-regs-only -"
	.ascii	"mbranch-protection=none -march=armv8-a+cr"
	.string	"c -mlittle-endian -mabi=lp64 -g -Os -std=gnu11 -fno-builtin -ffreestanding -fshort-wchar -fno-strict-aliasing -fno-stack-protector -fno-delete-null-pointer-checks -fstack-usage -fno-pic -ffunction-sections -fdata-sections -ffixed-r9 -fno-common -ffixed-x18"
.LASF12:
	.string	"long double"
.LASF10:
	.string	"short int"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"lib/asm-offsets.c"
.LASF1:
	.string	"/home/main/Documents/board/BAO_hypervisor/Linux_FreeRTOS/bao-demos/bao-demos/wrkdir/srcs/u-boot"
	.ident	"GCC: (GNU Toolchain for the Arm Architecture 11.2-2022.02 (arm-11.14)) 11.2.1 20220111"
