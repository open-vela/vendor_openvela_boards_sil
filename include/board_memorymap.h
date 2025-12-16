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

#ifndef __VENDOR_OPENVELA_BOARDS_SIL_INCLUDE_BOARD_MEMORYMAP_H
#define __VENDOR_OPENVELA_BOARDS_SIL_INCLUDE_BOARD_MEMORYMAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DDR start address */

#define QEMU_DDR_BASE             (0x40000000)
#define QEMU_DDR_SIZE             (0x40000000)

/* Kernel code memory (RX) */

#define KFLASH_START              (uintptr_t)__kflash_start
#define KFLASH_SIZE               (uintptr_t)__kflash_size
#define KFLASH_END                (KFLASH_START + KFLASH_SIZE)

/* Kernel RAM (RW) */

#define KSRAM_START               (uintptr_t)__ksram_start
#define KSRAM_SIZE                (uintptr_t)__ksram_size
#define KSRAM_END                 (KSRAM_START + KSRAM_SIZE)

/* Kernel calibration (RW) */
#define KCALIB_START              (uintptr_t)__kcalib_start
#define KCALIB_SIZE               (uintptr_t)__kcalib_size
#define KCALIB_END                (KCALIB_START + KCALIB_SIZE)

/* User code memory (RX) */

#define UFLASH_START              (uintptr_t)__uflash_start
#define UFLASH_SIZE               (uintptr_t)__uflash_size
#define UFLASH_END                (UFLASH_START + UFLASH_SIZE)

/* User RAM (RW) */

#define USRAM_START               (uintptr_t)__usram_start
#define USRAM_SIZE                (uintptr_t)__usram_size
#define USRAM_END                 (USRAM_START + USRAM_SIZE)

/* User calibration (RW) */
#define UCALIB_START              (uintptr_t)__ucalib_start
#define UCALIB_SIZE               (uintptr_t)__ucalib_size
#define UCALIB_END                (UCALIB_START + UCALIB_SIZE)

/* User bsw_manual_asil (RW) */
#define UBSW_MANUAL_ASIL_START    (uintptr_t)__bsw_manual_asil_start
#define UBSW_MANUAL_ASIL_SIZE     (uintptr_t)__bsw_asil_size
#define UBSW_MANUAL_ASIL_END      (UBSW_MANUAL_ASIL_START + UBSW_MANUAL_ASIL_SIZE)

/* User bsw_manual_qm (RW) */
#define UBSW_MANUAL_QM_START      (uintptr_t)__bsw_manual_qm_start
#define UBSW_MANUAL_QM_SIZE       (uintptr_t)__bsw_manual_qm_size
#define UBSW_MANUAL_QM_END        (UBSW_MANUAL_QM_START + UBSW_MANUAL_QM_SIZE)

/* User rte_asil (RW) */
#define URTE_ASIL_START           (uintptr_t)__rte_asil_start
#define URTE_ASIL_SIZE            (uintptr_t)__rte_asil_size
#define URTE_ASIL_END             (URTE_ASIL_START + URTE_ASIL_SIZE)

/* User rte_qm (RW) */
#define URTE_QM_START             (uintptr_t)__rte_qm_start
#define URTE_QM_SIZE              (uintptr_t)__rte_qm_size
#define URTE_QM_END               (URTE_QM_START + URTE_QM_SIZE)

/* User rpmsg_buffer (RW) */
#define URPMSG_BUFFER_START       (uintptr_t)__rpmsg_buffer_start
#define URPMSG_BUFFER_SIZE        (uintptr_t)__rpmsg_buffer_size
#define URPMSG_BUFFER_END         (URPMSG_BUFFER_START + URPMSG_BUFFER_SIZE)

/* User doip_diag_gateway (RW) */
#define UDOIP_DIAG_GATEWAY_START  (uintptr_t)__doip_diag_gateway_start
#define UDOIP_DIAG_GATEWAY_SIZE   (uintptr_t)__doip_diag_gateway_size
#define UDOIP_DIAG_GATEWAY_END    (UDOIP_DIAG_GATEWAY_START + UDOIP_DIAG_GATEWAY_SIZE)

/* User xcp_share (RW) */
#define UXCP_SHARE_START          (uintptr_t)__xcp_share_start
#define UXCP_SHARE_SIZE           (uintptr_t)__xcp_share_size
#define UXCP_SHARE_END            (UXCP_SHARE_START + UXCP_SHARE_SIZE)

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Kernel code memory (RX)  */

extern uint8_t          __kflash_start[];
extern uint8_t          __kflash_size[];

/* Kernel RAM (RW) */

extern uint8_t          __ksram_start[];
extern uint8_t          __ksram_size[];

/* Kernel calibration (RW) */

extern uint8_t          __kcalib_start[];
extern uint8_t          __kcalib_size[];

/* User code memory (RX) */

extern uint8_t          __uflash_start[];
extern uint8_t          __uflash_size[];

/* User RAM (RW) */

extern uint8_t          __usram_start[];
extern uint8_t          __usram_size[];

/* User calibration (RW) */

extern uint8_t          __ucalib_start[];
extern uint8_t          __ucalib_size[];


/* User bsw_manual_asil (RW) */
extern uint8_t          __bsw_manual_asil_start[];
extern uint8_t          __bsw_asil_size[];

/* User bsw_manual_asil (RW) */
extern uint8_t          __bsw_manual_qm_start[];
extern uint8_t          __bsw_manual_qm_size[];

/* User bsw_manual_asil (RW) */
extern uint8_t          __rte_asil_start[];
extern uint8_t          __rte_asil_size[];

/* User rte_qm (RW) */
extern uint8_t          __rte_qm_start[];
extern uint8_t          __rte_qm_size[];

/* User rpmsg_buffer (RW) */
extern uint8_t          __rpmsg_buffer_start[];
extern uint8_t          __rpmsg_buffer_size[];


/* User doip_diag_gateway (RW) */
extern uint8_t          __doip_diag_gateway_start[];
extern uint8_t          __doip_diag_gateway_size[];


/* User xcp_share (RW) */
extern uint8_t          __xcp_share_start[];
extern uint8_t          __xcp_share_size[];

#endif /* __VENDOR_OPENVELA_BOARDS_SIL_INCLUDE_BOARD_MEMORYMAP_H */
