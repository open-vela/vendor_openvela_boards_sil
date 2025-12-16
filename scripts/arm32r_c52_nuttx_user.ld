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
SECTIONS
{
    .userspace : {
        KEEP(*(.userspace))
    } > uflash

    .text : {
        _stext = ABSOLUTE(.);
        *(.text .text.*)
        *(.fixup)
        *(.gnu.warning)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.t.*)
        *(.glue_7)
        *(.glue_7t)
        *(.got)
        *(.gcc_except_table)
        *(.gnu.linkonce.r.*)
        _etext = ABSOLUTE(.);
    } > uflash

    .init_section : {
        _sinit = ABSOLUTE(.);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        _einit = ABSOLUTE(.);
    } > uflash

    .ARM.extab : {
        *(.ARM.extab*)
    } > uflash

    __exidx_start = ABSOLUTE(.);
    .ARM.exidx : {
        *(.ARM.exidx*)
    } > uflash

    __exidx_end = ABSOLUTE(.);

    .data : {
        . = ALIGN(64);
        _sdata = ABSOLUTE(.);
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        CONSTRUCTORS
        . = ALIGN(64);
        _edata = ABSOLUTE(.);
    } > usram AT > uflash

    _eronly = LOADADDR(.data);

    .bss : {
        _sbss = ABSOLUTE(.);
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(64);
        _ebss = ABSOLUTE(.);
    } > usram

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

    .spinlock_loc (NOLOAD) : ALIGN(4)
    {
        _sspinlock_loc = ABSOLUTE(.);
        KEEP (*(.spinlock_loc))
        . = ALIGN(4);
        _espinlock_loc = ABSOLUTE(.);
    } > app_manual_qm

    .ioc_app_manual_qm (NOLOAD) : ALIGN(4)
    {
        _sioc_app_manual_qm = ABSOLUTE(.);
        KEEP (*(.ioc_app_manual_qm))
        . = ALIGN(4);
        _eioc_app_manual_qm = ABSOLUTE(.);
    } > app_manual_qm

    .ioc_bsw_manual_qm (NOLOAD) : ALIGN(4)
    {
        _sioc_bsw_manual_qm = ABSOLUTE(.);
        KEEP (*(.ioc_bsw_manual_qm))
        . = ALIGN(4);
        _eioc_bsw_manual_qm = ABSOLUTE(.);
    } > bsw_manual_qm

    .ioc_rte_qm (NOLOAD) : ALIGN(4)
    {
        _sioc_rte_qm = ABSOLUTE(.);
        KEEP (*(.ioc_rte_qm.*))
        . = ALIGN(4);
        _eioc_rte_qm = ABSOLUTE(.);
    } > rte_qm

    .calibra : ALIGN(4)
    {
        _scalibra = ABSOLUTE(.);
        *(.calibra .calibra.*)
        . = ALIGN(4);
        _ecalibra = ABSOLUTE(.);
    } > ucalib AT > uflash

    _calib_pflash = LOADADDR(.calibra);

    /* Stabs debugging sections */

    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_info 0 : { *(.debug_info) }
    .debug_line 0 : { *(.debug_line) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_aranges 0 : { *(.debug_aranges) }

    __uflash_start = ORIGIN(uflash);
    __uflash_size = LENGTH(uflash);
    __usram_start = ORIGIN(usram);
    __usram_size = LENGTH(usram);
    __ucalib_start = ORIGIN(ucalib);
    __ucalib_size = LENGTH(ucalib);
}
