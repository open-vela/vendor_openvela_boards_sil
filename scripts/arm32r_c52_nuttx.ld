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

#include "arm32r_c52_memory.ld"

OUTPUT_ARCH(arm)
EXTERN(_vectors)
ENTRY(__start)

SECTIONS
{

  /* where the global variable out-of-bounds detection information located */

#ifdef CONFIG_MM_KASAN_GLOBAL
  .kasan.unused : {
    *(.data..LASANLOC*)
  }
  .kasan.global : {
    KEEP (*(.data..LASAN0))
    KEEP (*(.data.rel.local..LASAN0))
  }
  .kasan.shadows : {
    *(.kasan.shadows)
  }
#endif

  .text : {
      _stext = .;            /* Text section */
      __text_start = .;
      KEEP(*(.hyp_vectors))
      . = ALIGN(32);
      KEEP(*(.sys_vectors))
      *(.vectors)
      *(.text*)
      *(.fixup)
      *(.gnu.warning)
  } > kflash

  . = ALIGN(4096);

  .init_section : {
      _sinit = ABSOLUTE(.);
      KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
      KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
      _einit = ABSOLUTE(.);
      . = ALIGN(4096);
      _etext = .; /* End_1 of .text */
      _sztext = _etext - _stext;
  } > kflash

  .ARM.extab :
  {
      *(.ARM.extab*)
  } > kflash

  /* .ARM.exidx is sorted, so has to go in its own output section.  */

  PROVIDE_HIDDEN (__exidx_start = .);
  .ARM.exidx :
  {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > kflash
  PROVIDE_HIDDEN (__exidx_end = .);

  .rodata : {
      _srodata = .;          /* Read-only data */
      *(.rodata*)
      *(.data.rel.ro*)
      KEEP(*(SORT(.scattered_array*)));
      . = ALIGN(4096);
      _erodata = .;
  } > kflash

  .data : {                    /* Data */
      _sdata = .;
      *(.data*)
      . = ALIGN(8);
      __start_impls = .;
      *(.impls)
      KEEP(*(.impls))
      . = ALIGN(4);
      __stop_impls = .;
      _edata = .;
  } > ksram AT > kflash

  _eronly = LOADADDR(.data);

  .noinit : {
      _snoinit = ABSOLUTE(.);
      *(.noinit*)
      _enoinit = ABSOLUTE(.);
  } > ksram

  .bss : {                     /* BSS */
       _sbss = .;
       *(.bss*)
       . = ALIGN(4096);
       _ebss = .;
  } > ksram

  .boot_share (NOLOAD) : ALIGN(4)
  {
      _sboot_share = ABSOLUTE(.);
      KEEP (*(.boot_share))
      . = ALIGN(4);
      _eboot_share = ABSOLUTE(.);
  } > boot_share

  .standbyram (NOLOAD) : ALIGN(4)
  {
    _sstandby_ram = ABSOLUTE(.);
    *(.runtime_overflow .runtime_overflow.*)
    *(.no_init)
    . = ALIGN(4);
    _estandby_ram = ABSOLUTE(.);
  } > standby_ram

  .standbyramfix (NOLOAD) : ALIGN(4)
  {
    _sstandby_ram_fix = ABSOLUTE(.);
    *(.standby_ramfix .standby_ramfix.*)
    . = ALIGN(4);
    _estandby_ram_fix = ABSOLUTE(.);
  } > standby_ram_fix

  .ioc_bsw_manual_asil :  ALIGN(4)
  {
    _sioc_bsw_manual_asil = ABSOLUTE(.);
    KEEP(*(SORT(.ioc_bsw_manual_asil)))
    *(.rptun_shmem*)
    CONSTRUCTORS
    . = ALIGN(8);
    _eioc_bsw_manual_asil = ABSOLUTE(.);
  } > bsw_manual_asil AT > kflash

  .sync_barrier_loc (NOLOAD) : ALIGN(4)
  {
      _ssync_barrier_loc = ABSOLUTE(.);
      KEEP (*(.sync_barrier_loc))
      . = ALIGN(4);
      _esync_barrier_loc = ABSOLUTE(.);
  } > bsw_manual_asil

  .ioc_rte_qm : ALIGN(4)
  {
    _sioc_rte_qm = ABSOLUTE(.);
    KEEP(*(SORT(.ioc_rte_qm.*)))
    CONSTRUCTORS
    . = ALIGN(8);
    _eioc_rte_qm = ABSOLUTE(.);
  } > rte_qm AT > kflash

#ifdef CONFIG_BUILD_PROTECTED
  .spinlock_loc (NOLOAD) : ALIGN(4)
  {
      _sspinlock_loc = ABSOLUTE(.);
      KEEP (*(.spinlock_loc))
      . = ALIGN(4);
      _espinlock_loc = ABSOLUTE(.);
  } > app_manual_qm

  .ioc_app_manual_qm : ALIGN(4)
  {
      _sioc_app_manual_qm = ABSOLUTE(.);
      KEEP (*(.ioc_app_manual_qm))
      . = ALIGN(4);
      _eioc_app_manual_qm = ABSOLUTE(.);
  } > app_manual_qm AT> kflash

  .ioc_bsw_manual_qm (NOLOAD) : ALIGN(4)
  {
      _sioc_bsw_manual_qm = ABSOLUTE(.);
      KEEP (*(.ioc_bsw_manual_qm))
      . = ALIGN(4);
      _eioc_bsw_manual_qm = ABSOLUTE(.);
  } > bsw_manual_qm

#else
  .spinlock_loc (NOLOAD) : ALIGN(4)
  {
      _sspinlock_loc = ABSOLUTE(.);
      KEEP (*(.spinlock_loc))
      . = ALIGN(4);
      _espinlock_loc = ABSOLUTE(.);
  } > rte_asil
#endif

  .calibra : ALIGN(4)
  {
    _scalibra = ABSOLUTE(.);
    *(.calibra .calibra.*)
    . = ALIGN(4);
    _ecalibra = ABSOLUTE(.);
  } > kcalib AT > kflash

  _calib_pflash = LOADADDR(.calibra);

  .doip_diag_gateway : ALIGN(4)
  {
      _sdoip_diag_gateway = ABSOLUTE(.);
      KEEP (*(.doip_diag_gateway))
      . = ALIGN(4);
      _edoip_diag_gateway = ABSOLUTE(.);
  } > doip_diag_gateway AT> kflash

  .doip_diagserver (NOLOAD) : ALIGN(4)
  {
      _sdoip_diagserver = ABSOLUTE(.);
      KEEP (*(.doip_diagserver))
      . = ALIGN(4);
      _edoip_diagserver = ABSOLUTE(.);
  } > doip_diagserver

  .doip_manage (NOLOAD) : ALIGN(4)
  {
      _sdoip_manage = ABSOLUTE(.);
      KEEP (*(.doip_manage))
      . = ALIGN(4);
      _edoip_manage = ABSOLUTE(.);
  } > doip_manage

  .comcan_msg_share (NOLOAD) : ALIGN(4)
  {
      _scomcan_msg_share = ABSOLUTE(.);
      KEEP (*(.comcan_msg_share))
      . = ALIGN(4);
      _ecomcan_msg_share = ABSOLUTE(.);
  } > comcan_msg_share

#if CONFIG_CPU_COREID == 0
  .ioc_rte_asil : ALIGN(4)
  {
      _sioc_rte_asil = ABSOLUTE(.);
      KEEP (*(.ioc_rte_asil))
      . = ALIGN(4);
      _eioc_rte_asil = ABSOLUTE(.);
  } > rte_asil AT> kflash

  .xcp_share : ALIGN(4)
  {
      _sxcp_share = ABSOLUTE(.);
      KEEP (*(.xcp_share))
      . = ALIGN(4);
      _excp_share = ABSOLUTE(.);
  } > xcp_share AT> kflash

  .coredump : ALIGN(4)
  {
      _scoredump = ABSOLUTE(.);
      KEEP(*(SORT(.sync_barrier)))
      . = ALIGN(4);
      _ecoredump = ABSOLUTE(.);
  } > coredump AT> kflash

#else
  .ioc_rte_asil (NOLOAD) : ALIGN(4)
  {
      _sioc_rte_asil = ABSOLUTE(.);
      KEEP (*(.ioc_rte_asil))
      . = ALIGN(4);
      _eioc_rte_asil = ABSOLUTE(.);
  } > rte_asil

  .xcp_share (NOLOAD) : ALIGN(4)
  {
      _sxcp_share = ABSOLUTE(.);
      KEEP (*(.xcp_share))
      . = ALIGN(4);
      _excp_share = ABSOLUTE(.);
  } > xcp_share

  .coredump (NOLOAD) : ALIGN(4)
  {
      _scoredump = ABSOLUTE(.);
      KEEP(*(SORT(.sync_barrier)))
      . = ALIGN(4);
      _ecoredump = ABSOLUTE(.);
  } > coredump
#endif

  /* Sections to be discarded */
  /DISCARD/ : {
       *(.exit.text)
       *(.exit.data)
       *(.exitcall.exit)
  }

  .copy_table : ALIGN(4)
  {
    __copy_table = .;
#if (CONFIG_CPU_COREID == 0)
    LONG(LOADADDR(.ioc_rte_qm));
    LONG(_sioc_rte_qm);
    LONG(_eioc_rte_qm - _sioc_rte_qm);
    LONG(LOADADDR(.ioc_rte_asil));
    LONG(_sioc_rte_asil);
    LONG(_eioc_rte_asil - _sioc_rte_asil);
    LONG(LOADADDR(.xcp_share));
    LONG(_sxcp_share);
    LONG(_excp_share - _sxcp_share);
#endif
    LONG(LOADADDR(.calibra));
    LONG(_scalibra);
    LONG(_ecalibra - _scalibra);
    LONG(-1);
    LONG(-1);
    LONG(-1);
    __ecopy_table = .;
  } > kflash

  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }

  __kflash_start = ORIGIN(kflash);
  __kflash_size = LENGTH(kflash);
  __ksram_start = ORIGIN(ksram);
  __ksram_size = LENGTH(ksram);
  __rte_asil_start = ORIGIN(rte_asil);
  __rte_asil_size = LENGTH(rte_asil);
  __rte_qm_start = ORIGIN(rte_qm);
  __rte_qm_size = LENGTH(rte_qm);
  __bsw_manual_asil_start = ORIGIN(bsw_manual_asil);
  __bsw_asil_size = LENGTH(bsw_manual_asil);
  __bsw_manual_qm_start = ORIGIN(bsw_manual_qm);
  __bsw_manual_qm_size = LENGTH(bsw_manual_qm);
  __kcalib_start = ORIGIN(kcalib);
  __kcalib_size = LENGTH(kcalib);
  __standby_ram_start = ORIGIN(standby_ram);
  __standby_ram_size = LENGTH(standby_ram);
  __standby_ram_fix_start = ORIGIN(standby_ram_fix);
  __standby_ram_fix_size = LENGTH(standby_ram_fix);
  __rpmsg_buffer_start = ORIGIN(rpmsg_buffer);
  __rpmsg_buffer_size = LENGTH(rpmsg_buffer);
  __doip_diag_gateway_start = ORIGIN(doip_diag_gateway);
  __doip_diag_gateway_size = LENGTH(doip_diag_gateway);
  __xcp_share_start = ORIGIN(xcp_share);
  __xcp_share_size = LENGTH(xcp_share);

  #ifdef CONFIG_BUILD_PROTECTED
  __uflash_start = ORIGIN(uflash);
  __uflash_size = LENGTH(uflash);
  __usram_start = ORIGIN(usram);
  __usram_size = LENGTH(usram);
  __ucalib_start = ORIGIN(ucalib);
  __ucalib_size = LENGTH(ucalib);
  #endif
}

