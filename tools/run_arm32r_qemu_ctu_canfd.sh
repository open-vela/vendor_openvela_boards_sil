#!/bin/bash
#############################################################################
# Copyright (C) 2025 Xiaomi Corporation
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
#############################################################################
script_dir=$(dirname "$(readlink -f "$0")")

# Default number of CAN interfaces
CAN_COUNT=${1:-2}
# Ensure that the specified CAN count is a positive integer
if ! [[ "$CAN_COUNT" =~ ^[0-9]+$ ]] || [ "$CAN_COUNT" -lt 1 ]; then
    echo "Please provide a positive integer for the number of CAN interfaces."
    echo "The number of CAN interfaces must be at least 1."
    exit 1
fi

SERIAL_NUM=${2:-3}

BUILD_MODE=${3:-""}

CORE_NUM=${4:-2}

ETH_MODE=${6:-0}

ELF_PATH="./nuttx/nuttx"
ELF_USER_PATH="./nuttx/nuttx_user"

# nuttx.bin daily test construct package
if [ ! -f "$ELF_PATH" ]; then
ELF_PATH="${script_dir}/../nuttx"
ELF_USER_PATH="${script_dir}/../nuttx_user"
fi

if [ ! -f "$ELF_PATH" ]; then
ELF_PATH="${script_dir}/../vela_sil.elf"
ELF_USER_PATH="${script_dir}/../vela_sil_user.elf"
fi

# vela_core0.elf multiple core daily test construct package
if [ ! -f "$ELF_PATH" ]; then
ELF_PATH="${script_dir}/../vela_core0.elf"
ELF_USER_PATH="${script_dir}/../vela_core0_user.elf"
fi

BOOTLOADER_PATH="./nuttx/vela_bl.elf"

# vela_bl.elf daily test construct package
if [ ! -f "$BOOTLOADER_PATH" ]; then
BOOTLOADER_PATH="${script_dir}/../vela_bl.elf"
fi

HOST_9PFS_DIR="./tmp/host"
mkdir -p $HOST_9PFS_DIR
echo "Create an default directory: $HOST_9PFS_DIR"

# path of qemu-system-arm

QEMU_PATH="./prebuilts/qemu/linux-x86_64/"
MULTI_PROTECT_ELF_PATH="./out/cur-release/"

# create boot directory
HOST_BOOT_DIR="${script_dir}/../../boot_path/"
if [ ! -d "$HOST_BOOT_DIR" ]; then
    mkdir -p "$HOST_BOOT_DIR"
fi
cp -r "${script_dir}/../"  "$HOST_BOOT_DIR"
# uniform nuttx and vela_sil to vela_core0
if [ -f "$HOST_BOOT_DIR/vela_sil.elf" ]; then
    cp "$HOST_BOOT_DIR/vela_sil.elf" "$HOST_BOOT_DIR/vela_core0.elf"
    ELF_PATH="$HOST_BOOT_DIR/vela_core0.elf"
fi
if [ -f "$HOST_BOOT_DIR/vela_sil_user.elf" ]; then
    cp "$HOST_BOOT_DIR/vela_sil_user.elf" "$HOST_BOOT_DIR/vela_core0_user.elf"
    ELF_USER_PATH="$HOST_BOOT_DIR/vela_core0_user.elf"
fi

# Basic QEMU start parameters
QEMU_CMD="$QEMU_PATH/bin/qemu-system-arm -L $QEMU_PATH/share/qemu/ -cpu cortex-r52 -semihosting -machine virt,virtualization=on,gic-version=2,highmem=off -m 1G
	  -nographic -serial mon:stdio -s"

# Eth adapt
listen_pot="12345"
# socket mode
if [ "$ETH_MODE" == "0" ]; then
    echo "=== Qemu \"socket\" mode used"
    echo "host socket: tcp://localhost:$listen_pot"
    QEMU_CMD="$QEMU_CMD -netdev socket,id=b1,listen=:$listen_pot -device virtio-net-device,netdev=b1"
# tap/bridge mode -- need run simhostroute.sh script before
elif [ "$ETH_MODE" == "1" ]; then
    echo "=== Qemu \"tap/bridge\" mode used"
    echo "please make sure nuttx/tools/simhostroute.sh script has been run !!"
    if ! ip link show nuttx0 &>/dev/null; then
        echo "Error: bridge nuttx0 does not exist. Please run script first !"
        exit 1
    fi
    QEMU_CMD="$QEMU_CMD -device virtio-net-device,netdev=b1 -netdev bridge,br=nuttx0,id=b1,helper=/usr/local/libexec/qemu-bridge-helper"
# user/default mode
elif [ "$ETH_MODE" == "2" ]; then
    echo "=== Qemu \"user default\" mode used"
    QEMU_CMD="$QEMU_CMD -device virtio-net-device,netdev=b1 -netdev user,id=b1"
# unknown parameter
else
    echo "unknown parameter"
    echo "Usage: $0 $1 $2 $3 $4 $5 $ETH_MODE"
    echo "eth mode: 0- socket(default), 1- tap/bridge, 2- user/default"
    echo "build mode: cmake(default), makefile"
    exit 1
fi

# load image
if [ ! -f "$BOOTLOADER_PATH" ]; then
    if [ "$BUILD_MODE" == "protect" ]; then
        if [ "$CORE_NUM" -gt 1 ]; then
            QEMU_CMD="$QEMU_CMD -device loader,file=$MULTI_PROTECT_ELF_PATH/vela_core0_user.elf \
                -device loader,file=$MULTI_PROTECT_ELF_PATH/vela_core0.elf"
        else
            QEMU_CMD="$QEMU_CMD -device loader,file=$ELF_USER_PATH \
                -device loader,file=$ELF_PATH"
        fi
    else
        QEMU_CMD="$QEMU_CMD -kernel $ELF_PATH"
    fi
else
    QEMU_CMD="$QEMU_CMD -kernel $BOOTLOADER_PATH"
fi

core0_can_channel=$(( CAN_COUNT / 3 * 2 ))
echo "core0 has $core0_can_channel can channels"

for ((i=0; i<$CAN_COUNT; i++)); do
    CAN_BUS_ID="canbus$i-bus"
    CAN_HOST_ID="canhost$i"
    CAN_INTERFACE="can$i"
    #Check if each CAN interface exists
    if ! ip link show "$CAN_INTERFACE" &>/dev/null; then
        echo "Error: CAN interface $CAN_INTERFACE does not exist. Please check !"
        exit 1
    fi
    QEMU_CMD="$QEMU_CMD -object can-bus,id=$CAN_BUS_ID
            -object can-host-socketcan,id=$CAN_HOST_ID,if=can$i,canbus=$CAN_BUS_ID"
    if [ $(($i % 2)) -ne 0 ]; then
        if (( i < core0_can_channel )); then
            QEMU_CMD="$QEMU_CMD -device ctucan_pci,canbus0=$CAN_BUS_ID_LAST,canbus1=$CAN_BUS_ID,bus=pcie.0"
        else
            QEMU_CMD="$QEMU_CMD -device ctucan_pci,canbus0=$CAN_BUS_ID_LAST,canbus1=$CAN_BUS_ID,bus=pcie.1"
        fi
    fi
    CAN_BUS_ID_LAST=$CAN_BUS_ID
done
if [ $((CAN_COUNT % 2)) -ne 0 ]; then
    QEMU_CMD="$QEMU_CMD -device ctucan_pci,canbus0=$CAN_BUS_ID_LAST,canbus1=$CAN_BUS_ID_LAST,bus=pcie.1"
fi

# add virtio-serial
configure_serial_devices() {
    local serial_count=$1
    local qemu_cmd=$2
    local virtio_bus_index=0

    for ((i = 0; i < serial_count; i++)); do
        bus_id="virtio-mmio-bus.$virtio_bus_index"
        port=$((3450 + i))
        chardev_id="foo$i"

        # append parm "virtio-serial and virtconsole"
        qemu_cmd+=" -device virtio-serial-device,bus=$bus_id "
        qemu_cmd+=" -chardev socket,telnet=off,host=127.0.0.1,port=$port,server=on,wait=off,id=$chardev_id "
        qemu_cmd+=" -device virtconsole,chardev=$chardev_id "

        # increase virtio bus index
        virtio_bus_index=$((virtio_bus_index + 1))
    done
    echo "$qemu_cmd"
}
QEMU_CMD=$(configure_serial_devices "$SERIAL_NUM" "$QEMU_CMD")

QEMU_CMD="$QEMU_CMD -blockdev node-name=pflash1,driver=file,filename=$script_dir/flash.img -M virt,pflash1=pflash1"

# coredump
if [ "$BUILD_MODE" = "protect" ] && [ "$CORE_NUM" -gt 1 ]; then
  COREDUMP_FILE_PATH=$MULTI_PROTECT_ELF_PATH
else
  COREDUMP_FILE_PATH=$(dirname $ELF_PATH)
fi

COREDUMP_FILE="$COREDUMP_FILE_PATH/coredump.core"

if [ -e $COREDUMP_FILE ]; then
  core_format=$(file $COREDUMP_FILE)
  if [ -n "$(echo ${core_format} | grep 'core file')" ]; then
      timesamp=$(date +%Y%m%d%H%M%S)
      mv $COREDUMP_FILE $COREDUMP_FILE_PATH/${timesamp}.core
      dd if=/dev/zero of=$COREDUMP_FILE bs=32MB count=1
      echo "A core file already exists. will be backed to ${timesamp}.core"
  fi
else
  echo "Create a core file"
  dd if=/dev/zero of=$COREDUMP_FILE bs=32MB count=1
fi

if [ -e $COREDUMP_FILE ]; then
  coredump_io_idx=$((SERIAL_NUM + 1))
  COREDUMP_OPTION="-drive index=2,id=vendor,if=none,format=raw,file=$COREDUMP_FILE -device virtio-blk-device,bus=virtio-mmio-bus.$coredump_io_idx,drive=vendor"
else
  echo "Warning: $COREDUMP_FILE create fail, please check!"
  COREDUMP_OPTION=""
fi

NPD_OPTION="-M npd=2"

QEMU_CMD="$QEMU_CMD $COREDUMP_OPTION $NPD_OPTION"

# Execute the QEMU start command
echo "The command to be executed is:"
echo $QEMU_CMD  | fold -w 150
$QEMU_CMD
