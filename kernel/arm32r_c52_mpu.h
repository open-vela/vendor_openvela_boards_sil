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

#ifndef __VENDOR_OPENVELA_BOARDS_SIL_KERNEL_ARM32R_C52_MPU_H
#define __VENDOR_OPENVELA_BOARDS_SIL_KERNEL_ARM32R_C52_MPU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include "mpu.h"

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/* User calibration (RW) */
#define mpu_user_calib(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)

/* Kernel calibration (RW) */
#define mpu_priv_calib(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_AP_RWNO, \
                       MPU_RLAR_NONCACHEABLE)

/* User bsw_manual_qm (RW) */
#define mpu_user_bsw_manual_qm(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)

/* User rte_qm (RW) */
#define mpu_user_rte_qm(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)

/* User rpmsg_buffer (RW) */
#define mpu_user_rpmsg_buffer(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)

/* User doip_diag_gateway (RW) */
#define mpu_user_doip_diag_gateway(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)


/* User xcp_share (RW) */
#define mpu_user_xcp_share(base, size) \
  /* The configure the region */ \
  mpu_configure_region(base, size, \
                       MPU_RBAR_XN | \
                       MPU_RBAR_AP_RWRW, \
                       MPU_RLAR_NONCACHEABLE)

/****************************************************************************
 * Name: arm32r_c52_mpu_init
 *
 * Description:
 *   Configure the MPU to for different RAM region allocation.
 *
 ****************************************************************************/

#if defined(CONFIG_ARCH_USE_MPU)
void arm32r_c52_mpu_init(void);
#endif

#endif /* __VENDOR_OPENVELA_BOARDS_SIL_KERNEL_ARM32R_C52_MPU_H */