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

#include "memory_layout.h"

MEMORY
{
  kflash            (rx) : ORIGIN = KERNEL_PFLASH_START,            LENGTH = KERNEL_PFLASH_SIZE
  ksram             (rw) : ORIGIN = KERNEL_SRAM_START,              LENGTH = KERNEL_SRAM_SIZE
  boot_share        (rx) : ORIGIN = BOOT_SHARE_START,               LENGTH = BOOT_SHARE_SIZE
  coredump          (rw) : ORIGIN = COREDUMP_START,                 LENGTH = COREDUMP_SIZE
  standby_ram       (rw) : ORIGIN = STANDBY_RAM_START,              LENGTH = STANDBY_RAM_SIZE
  standby_ram_fix   (rw) : ORIGIN = STANDBY_RAM_FIX_START,          LENGTH = STANDBY_RAM_FIX_SIZE
  bsw_manual_asil   (rw) : ORIGIN = BSW_MANUAL_ASIL_START,          LENGTH = BSW_MANUAL_ASIL_SIZE
  rte_asil          (rw) : ORIGIN = RTE_ASIL_START,                 LENGTH = RTE_ASIL_SIZE
  bsw_manual_qm     (rw) : ORIGIN = BSW_MANUAL_QM_START,            LENGTH = BSW_MANUAL_QM_SIZE
  app_manual_qm     (rw) : ORIGIN = APP_MANUAL_QM_START,            LENGTH = APP_MANUAL_QM_SIZE
  rte_qm            (rw) : ORIGIN = RTE_QM_START,                   LENGTH = RTE_QM_SIZE
  kcalib            (rw) : ORIGIN = CALIB_KERNEL_START,             LENGTH = CALIB_KERNEL_SIZE
  rpmsg_buffer      (rw) : ORIGIN = RPMSG_SHMEM_START,              LENGTH = RPMSG_SHMEM_SIZE
  doip_diag_gateway (rw) : ORIGIN = SHARED_DOIP_DIAG_GATEWAY_START, LENGTH = SHARED_DOIP_DIAG_GATEWAY_SIZE
  doip_manage       (rw) : ORIGIN = SHARED_DOIP_MANAGER_START,      LENGTH = SHARED_DOIP_MANAGER_SIZE
  xcp_share         (rw) : ORIGIN = SHARED_XCP_START,               LENGTH = SHARED_XCP_SIZE
  doip_diagserver   (rw) : ORIGIN = SHARED_DOIP_DIAGSERVER_START,   LENGTH = SHARED_DOIP_DIAGSERVER_SIZE
  comcan_msg_share  (rw) : ORIGIN = SHARED_COMCAN_MSG_SHARE_START,  LENGTH = SHARED_COMCAN_MSG_SHARE_SIZE

  #ifdef CONFIG_BUILD_PROTECTED
  uflash            (rx) : ORIGIN = USER_PFLASH_START,              LENGTH = USER_PFLASH_SIZE
  usram             (rw) : ORIGIN = USER_SRAM_START,                LENGTH = USER_SRAM_SIZE
  ucalib            (rw) : ORIGIN = CALIB_USER_START,               LENGTH = CALIB_USER_SIZE
  #endif
}
