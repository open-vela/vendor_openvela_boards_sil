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

#include <nuttx/nuttx.h>
#include <nuttx/fdt.h>
#include <nuttx/kthread.h>
#include <nuttx/pci/pci_ecam.h>
#include <nuttx/virtio/virtio-mmio.h>
#include <nuttx/rptun/rptun.h>
#include <nuttx/serial/uart_rpmsg.h>
#include <nuttx/timers/arch_rtc.h>
#include <nuttx/timers/pl031.h>
#include <nuttx/timers/rpmsg_rtc.h>
#include <nuttx/timers/rtc.h>
#include <nuttx/mtd/mtd.h>
#include <nuttx/mtd/mtd_log.h>
#include <nuttx/mtd/configdata.h>
#include <sys/param.h>
#include <nuttx/lib/elf.h>

#ifdef CONFIG_LIBC_FDT
#  include <libfdt.h>
#endif

#ifdef CONFIG_TIMER_WDOG
#  include <nuttx/timers/timer_wdog.h>
#endif

#include "gic.h"
#ifdef CONFIG_ARCH_BOARD_CUSTOM_QEMU_ARM32
#include "memory_layout.h"
#endif

#include <debug.h>

#ifdef CONFIG_BUILD_PROTECTED
#include <arm32r_c52_userspace.h>
#endif

#ifdef CONFIG_ARCH_USE_MPU
#include "arm32r_c52_mpu.h"
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct part_table
{
  unsigned long long offset; /* partition offset */
  unsigned long long size;   /* partition size in byte */
  const char        *name;   /* name of the partition */
};

typedef volatile union
{
  uint8_t              *uc_ptr;
  uint16_t             *us_ptr;
  uint32_t             *ui_ptr;
  unsigned long long   *ull_ptr;
} copy_table_ptr;

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define QEMU_SPI_IRQ_BASE            32

/* Interrupt ids:
 * ID0-ID7 for Non-secure interrupts
 * ID8-ID15 for Secure interrupts.
 */

#define QEMU_NOSECURE_INTTERRUPT     0
#define QEMU_SECURE_INTTERRUPT       8

/* OpenAMP shared memory, this address must be not used by vela */

#ifndef CONFIG_ARM_BUSY_WAIT_FLAG_ADDR
#  define CONFIG_ARM_BUSY_WAIT_FLAG_ADDR 0x45000000
#endif

#define QEMU_SHMEM_ADDR              (CONFIG_ARM_BUSY_WAIT_FLAG_ADDR + 0x4)

#define FDT_PCI_TYPE_IO              0x01000000
#define FDT_PCI_TYPE_MEM32           0x02000000
#define FDT_PCI_TYPE_MEM64           0x03000000
#define FDT_PCI_TYPE_MASK            0x03000000
#define FDT_PCI_PREFTCH              0x40000000

#define QEMU_RSC_TABLE_SIZE          0x10000

#ifndef CONFIG_BOARD_MTDLOG_PATH
#  define CONFIG_BOARD_MTDLOG_PATH "/dev/mtdlog"
#endif

/****************************************************************************
 * Extern Functions declaration and values
 ****************************************************************************/

extern uint32_t __copy_table[];   /* copy table entry */

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct part_table g_mtd_partition_table[] =
{
#if CONFIG_CPU_COREID == 0
  {
    .offset = SLOT_A_FLASH_START,
    .size   = SLOT_A_FLASH_SIZE,
    .name   = "/dev/slot_a"
  },
  {
    .offset = SLOT_B_FLASH_START,
    .size   = SLOT_B_FLASH_SIZE,
    .name   = "/dev/slot_b"
  },
  {
    .offset   = FAKE_COREDUMP_START,
    .size     = FAKE_COREDUMP_SIZE,
    .name     = "/dev/fake_coredump",
  },
  {
    .offset = CORE0_DFLASH_MTDLOG_START,
    .size = CORE0_DFLASH_MTDLOG_SIZE,
    .name = CONFIG_BOARD_MTDLOG_PATH
  }
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_DRIVERS_VIRTIO_MMIO

/****************************************************************************
 * Name: register_virtio_devices_from_fdt
 ****************************************************************************/

static void register_virtio_devices_from_fdt(const void *fdt)
{
  uintptr_t addr;
  int offset = -1;
  int irqnum;

  for (; ; )
    {
      offset = fdt_node_offset_by_compatible(fdt, offset, "virtio,mmio");
      if (offset == -FDT_ERR_NOTFOUND)
        {
          break;
        }

      addr = fdt_get_reg_base(fdt, offset, 0);
      irqnum = fdt_get_irq(fdt, offset, 1, QEMU_SPI_IRQ_BASE);
      if (addr > 0 && irqnum >= 0)
        {
          virtio_register_mmio_device((void *)addr, irqnum);
        }
    }
}

#endif

/****************************************************************************
 * Name: qemu_partion_init
 * ***************************************************************************/

int qemu_partion_init(void)
{
  struct mtd_geometry_s geo;
  struct part_table    *part_info;
  struct mtd_dev_s     *mtd_part;
  struct mtd_dev_s     *mtd_flash;
  struct inode         *mtdnode;
  int ret;
  int i;

  if (find_mtddriver("/dev/cfi-flash1", &mtdnode) != OK)
  {
    ferr("not find mtd flash /dev/cfi-flash1!");
    return -1;
  }

  mtd_flash = mtdnode->u.i_mtd;

  MTD_IOCTL(mtd_flash, MTDIOC_GEOMETRY, (unsigned long)&geo);
  finfo("LittleFS: %ld blocks, %ld bytes/block, %ld erase blocks\n",
                   geo.neraseblocks, geo.blocksize, geo.erasesize);


  for (i = 0; i < nitems(g_mtd_partition_table); i++)
  {
    part_info = &g_mtd_partition_table[i];

    finfo("%s : offset = 0x%llx, size = 0x%llx\n", part_info->name,
           part_info->offset , part_info->size);
    mtd_part = mtd_partition(mtd_flash, part_info->offset /geo.blocksize,
                             part_info->size /geo.blocksize);

    if (mtd_part == NULL)
      {
        ferr("[%s]ERROR: mtd_partition() failed %d\n", part_info->name, errno);
        continue;
      }

#ifdef CONFIG_MTDLOG
    if (!strcmp(part_info->name, CONFIG_BOARD_MTDLOG_PATH))
      {
        ret = mtdlog_register(part_info->name, mtd_part);
      }
    else
#endif
      {
        ret = register_mtddriver(part_info->name, mtd_part, 0755, NULL);
      }

    if (ret < 0)
      {
        _err("register_mtddriver() failed: %d\n", ret);
      }
  }

  return OK;
}
/****************************************************************************
 * Name: register_devices_from_fdt
 ****************************************************************************/
#if defined(CONFIG_LIBC_FDT) && defined(CONFIG_DEVICE_TREE)
static void register_devices_from_fdt(void)
{
  const void *fdt = fdt_get();

  if (fdt == NULL)
    {
      return;
    }

#ifdef CONFIG_DRIVERS_VIRTIO_MMIO
  register_virtio_devices_from_fdt(fdt);
#endif

#ifdef CONFIG_PCI
  fdt_pci_ecam_register_domain(fdt, CONFIG_PCI_DOMAIN);
#endif

#ifdef CONFIG_MTD_CFI
  if (fdt_cfi_register(fdt) < 0)
    {
      syslog(LOG_ERR, "fdt_cfi_register failed\n");
    }

  if (sched_getcpu() == 0 || sched_getcpu() == 4)
    {
      qemu_partion_init();
    }
#endif
}
#endif

#ifdef CONFIG_RTC_PL031
int up_rtc_initialize(void)
{
  struct rtc_lowerhalf_s *lowerhalf;
  const void *fdt = fdt_get();
  bool sync = true;

  if (fdt == NULL)
    {
      return -EINVAL;
    }

  lowerhalf = pl031_initialize(fdt_get_reg_base_by_path(fdt, "/pl031"),
                               fdt_get_irq_by_path(fdt, 1, "/pl031",
                                                   QEMU_SPI_IRQ_BASE));

  up_rtc_set_lowerhalf(lowerhalf, sync);

  return rtc_initialize(0, lowerhalf);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int board_init_mmio(void)
{
#if defined(CONFIG_LIBC_FDT) && defined(CONFIG_DEVICE_TREE)
  register_devices_from_fdt();
#endif
#ifdef CONFIG_TIMER_WDOG
  if (timer_wdog_initialize(0) < 0)
    {
      serr("ERROR: Failed to initialize timer_wdog\n");
    }

  if (timer_wdog_initialize(2) < 0)
    {
      serr("ERROR: Failed to initialize timer_wdog\n");
    }
#endif
  return 0;
}

void board_mpu_init(void)
{
#ifdef CONFIG_ARCH_USE_MPU
  arm32r_c52_mpu_init();
#endif

#ifdef CONFIG_BUILD_PROTECTED
  /* Only the .calib segment is processed.
   * The .bss segment and the .data segment cannot be processed here.
   */
  arm32r_c52_userspace();
#endif
}

void board_init_ram_sections(void)
{
  uint32_t *table;
  uint32_t length, count;
  copy_table_ptr block_src, block_dest;

   /* copy table */
  table = (uint32_t *)&__copy_table;
  while (table)
    {
      block_src.ui_ptr  = (uint32_t *)*table++;
      block_dest.ui_ptr = (uint32_t *)*table++;
      length            = *table++;

      /* we are finished when length == -1 */
      if (length == 0xFFFFFFFF)
      {
        break;
      }
      count = length / 8;
      while (count--)
      {
        *block_dest.ull_ptr++ = *block_src.ull_ptr++;
      }
      if (length & 0x4)
      {
        *block_dest.ui_ptr++ = *block_src.ui_ptr++;
      }
      if (length & 0x2)
      {
        *block_dest.us_ptr++ = *block_src.us_ptr++;
      }
      if (length & 0x1)
      {
        *block_dest.uc_ptr = *block_src.uc_ptr;
      }
    }
}

/****************************************************************************
 * Name: board_early_basic_init
 ****************************************************************************/

int board_early_basic_init(void)
{
  board_mpu_init();

  /* Initialize the RAM sections, this must be done before the board
   * initialization.
   */

  board_init_ram_sections();

  return OK;
}

/****************************************************************************
 * Name: board_ioctl
 *
 * Description:
 *   This method is to handle the commands from boardctl.
 *
 * Input Parameters:
 *   cmd - The command from boardctl.
 *   arg - Extra argument from boardctl.
 *
 * Returned Value:
 *   result of executing a command.
 *
 ****************************************************************************/

int board_ioctl(unsigned int cmd, uintptr_t arg)
{
  return OK;
}

/****************************************************************************
 * Name: board_get_core_id_by_flash_size
 *
 * Description:
 *   The method of calculating the current core ID based on the flash load address.
 *
 * Input Parameters:
 *   loadinfo - elf file infomation
 *
 * Returned Value:
 *   result of core number.
 *
 ****************************************************************************/

int board_get_core_id_by_flash_size(struct mod_loadinfo_s *loadinfo)
{
  uint32_t addr = loadinfo->ehdr.e_entry;

  if (addr >= CORE0_KFLASH_START && addr < CORE0_KFLASH_START + CORE0_KFLASH_SIZE) {
      return 0;
  }

  return -1;
}
