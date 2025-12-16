/****************************************************************************
 * Copyright (C) 2025 Xiaomi Corporation
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __VENDOR_OPENVELA_BOARDS_SIL_SCRIPTS_MEMORY_LAYOUT_H
#define __VENDOR_OPENVELA_BOARDS_SIL_SCRIPTS_MEMORY_LAYOUT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "nuttx/config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MEMLAYOUT_SW_CFG_MAJOR_VERSION 2
#define MEMLAYOUT_SW_CFG_MINOR_VERSION 2
#define MEMLAYOUT_SW_CFG_PATCH_VERSION 3

#define BOOTLOADER_FLASH_START 0x00000000
#define BOOTLOADER_FLASH_SIZE 0x000C0000 /* 768KB */
#define MBF_HASH_START 0x000C0000
#define MBF_HASH_SIZE 0x00040000 /* 256KB */
#define CORE0_KFLASH_START 0x00100000
#define CORE0_KFLASH_SIZE 0x00500000 /* 5120KB */
#define CORE0_UFLASH_START 0x00600000
#define CORE0_UFLASH_SIZE 0x00200000 /* 2048KB */
#define CORE0_STANDBY_RAM_START 0x40200000
#define CORE0_STANDBY_RAM_SIZE 0x00001800 /* 6KB */
#define CORE0_STANDBY_RAM_FIX_START 0x40201800
#define CORE0_STANDBY_RAM_FIX_SIZE 0x00000800 /* 2KB */
#define CORE0_CALIB_KERNEL_START 0x40304000
#define CORE0_CALIB_KERNEL_SIZE 0x00008000 /* 32KB */
#define CORE0_CALIB_USER_START 0x4030C000
#define CORE0_CALIB_USER_SIZE 0x00008000 /* 32KB */
#define BSW_MANUAL_ASIL_START 0x40424000
#define BSW_MANUAL_ASIL_SIZE 0x00008000 /* 32KB */
#define APP_MANUAL_QM_START 0x4042C000
#define APP_MANUAL_QM_SIZE 0x00004000 /* 16KB */
#define BSW_MANUAL_QM_START 0x40430000
#define BSW_MANUAL_QM_SIZE 0x00004000 /* 16KB */
#define RTE_ASIL_START 0x40434000
#define RTE_ASIL_SIZE 0x00008000 /* 32KB */
#define RTE_QM_START 0x4043C000
#define RTE_QM_SIZE 0x00008000 /* 32KB */
#define RPMSG_SHMEM_START 0x40444000
#define RPMSG_SHMEM_SIZE 0x00020000 /* 128KB */
#define SHARED_DOIP_DIAG_GATEWAY_START 0x40464000
#define SHARED_DOIP_DIAG_GATEWAY_SIZE 0x00040000 /* 256KB */
#define SHARED_XCP_START 0x404A4000
#define SHARED_XCP_SIZE 0x00008000 /* 32KB */
#define BOOT_SHARE_START 0x404AC000
#define BOOT_SHARE_SIZE 0x00004000 /* 16KB */
#define COREDUMP_START 0x404B0000
#define COREDUMP_SIZE 0x00010000 /* 64KB */
#define SHARED_DOIP_MANAGER_START 0x404C0000
#define SHARED_DOIP_MANAGER_SIZE 0x00020000 /* 128KB */
#define SHARED_DOIP_DIAGSERVER_START 0x404E0000
#define SHARED_DOIP_DIAGSERVER_SIZE 0x00020000 /* 128KB */
#define SHARED_COMCAN_MSG_SHARE_START 0x40500000
#define SHARED_COMCAN_MSG_SHARE_SIZE 0x00020000 /* 128KB */

#define CORE0_KERNEL_SRAM_START CONFIG_RAM_START /* 0x41000000 */
#define CORE0_KERNEL_SRAM_SIZE CONFIG_RAM_SIZE /* 4096KB */
#define CORE0_USER_SRAM_START 0x41400000
#define CORE0_USER_SRAM_SIZE 0x00400000 /* 4096KB */

#define SLOT_A_FLASH_START 0x0
#define SLOT_A_FLASH_SIZE 0x01C00000 /* 28672KB */
#define SLOT_B_FLASH_START 0x01C00000
#define SLOT_B_FLASH_SIZE 0x01C00000 /* 28672KB */
#define CORE0_DFLASH_NVM_START 0x03800000
#define CORE0_DFLASH_NVM_SIZE 0x00100000 /* 1024KB */
#define CORE0_DFLASH_MTDLOG_START 0x03900000
#define CORE0_DFLASH_MTDLOG_SIZE 0x00100000 /* 1024KB */
#define CORECS_DFLASH_NVM_START 0x03A00000
#define CORECS_DFLASH_NVM_SIZE 0x00200000 /* 2048KB */
#define FAKE_COREDUMP_START 0x03E00000
#define FAKE_COREDUMP_SIZE 0x40000 /* 256KB */


/****************************************************************************
 * The memory layout address and size only for test
 *
 ****************************************************************************/


#if defined(CONFIG_CPU_COREID)
#if (CONFIG_CPU_COREID == 0)
#define KERNEL_PFLASH_START CORE0_KFLASH_START
#define KERNEL_PFLASH_SIZE CORE0_KFLASH_SIZE
#define KERNEL_SRAM_START CORE0_KERNEL_SRAM_START
#define KERNEL_SRAM_SIZE CORE0_KERNEL_SRAM_SIZE
#define CALIB_KERNEL_START CORE0_CALIB_KERNEL_START
#define CALIB_KERNEL_SIZE CORE0_CALIB_KERNEL_SIZE
#define STANDBY_RAM_START CORE0_STANDBY_RAM_START
#define STANDBY_RAM_SIZE CORE0_STANDBY_RAM_SIZE
#define STANDBY_RAM_FIX_START CORE0_STANDBY_RAM_FIX_START
#define STANDBY_RAM_FIX_SIZE CORE0_STANDBY_RAM_FIX_SIZE
#if defined(CONFIG_BUILD_PROTECTED)
#define USER_PFLASH_START CORE0_UFLASH_START
#define USER_PFLASH_SIZE CORE0_UFLASH_SIZE
#define USER_SRAM_START CORE0_USER_SRAM_START
#define USER_SRAM_SIZE CORE0_USER_SRAM_SIZE
#define CALIB_USER_START CORE0_CALIB_USER_START
#define CALIB_USER_SIZE CORE0_CALIB_USER_SIZE
#endif
#elif (CONFIG_CPU_COREID == -1)
#define KERNEL_PFLASH_START BOOTLOADER_FLASH_START
#define KERNEL_PFLASH_SIZE BOOTLOADER_FLASH_SIZE
#define KERNEL_SRAM_START CORE0_KERNEL_SRAM_START
#define KERNEL_SRAM_SIZE CORE0_KERNEL_SRAM_SIZE
#define CALIB_KERNEL_START CORE0_CALIB_KERNEL_START
#define CALIB_KERNEL_SIZE CORE0_CALIB_KERNEL_SIZE
#define STANDBY_RAM_START CORE0_STANDBY_RAM_START
#define STANDBY_RAM_SIZE CORE0_STANDBY_RAM_SIZE
#define STANDBY_RAM_FIX_START CORE0_STANDBY_RAM_FIX_START
#define STANDBY_RAM_FIX_SIZE CORE0_STANDBY_RAM_FIX_SIZE
#endif
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* __VENDOR_OPENVELA_BOARDS_SIL_SCRIPTS_MEMORY_LAYOUT_H */
