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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "arm32r_c52_mpu.h"
#include <arch/board/board_memorymap.h>
#include "mpu.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm32r_c52_mpu_init
 *
 * Description:
 *   Configure the MPU to for different RAM region allocation.
 *
 ****************************************************************************/

#if defined(CONFIG_ARCH_USE_MPU)
void arm32r_c52_mpu_init(void)
{
  mpu_user_calib(UCALIB_START, UCALIB_SIZE);
  mpu_user_bsw_manual_qm(UBSW_MANUAL_QM_START, UBSW_MANUAL_QM_SIZE);
  mpu_user_rte_qm(URTE_QM_START, URTE_QM_SIZE);
  mpu_user_rpmsg_buffer(URPMSG_BUFFER_START, URPMSG_BUFFER_SIZE);
  mpu_user_doip_diag_gateway(UDOIP_DIAG_GATEWAY_START, UDOIP_DIAG_GATEWAY_SIZE);
  mpu_user_xcp_share(UXCP_SHARE_START, UXCP_SHARE_SIZE);
}
#endif /* CONFIG_ARCH_USE_MPU */