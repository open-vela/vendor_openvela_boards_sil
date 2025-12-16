# ##############################################################################
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
# ##############################################################################

add_subdirectory(src)

if(CONFIG_ARCH_ARMV8R)
  set(LD_SCRIPT_FILE arm32r_c52_nuttx.ld)
endif()

set_property(GLOBAL PROPERTY LD_SCRIPT
                              "${NUTTX_BOARD_ABS_DIR}/scripts/${LD_SCRIPT_FILE}")

if(CONFIG_BUILD_PROTECTED)
  add_subdirectory(kernel)
  target_include_directories(board PRIVATE ${NUTTX_BOARD_ABS_DIR}/kernel)
  target_include_directories(board PRIVATE ${NUTTX_BOARD_ABS_DIR}/scripts)
  if(CONFIG_ARCH_ARMV8R)
    set_property(GLOBAL PROPERTY LD_SCRIPT_USER ${NUTTX_BOARD_ABS_DIR}/scripts/arm32r_c52_nuttx_user.ld)
  endif()
endif()

if(CONFIG_BUILD_PROTECTED)
  add_custom_target(
      nuttx_post_build
      DEPENDS nuttx systemmap usermap
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_OBJCOPY} -O binary nuttx vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.bin
      COMMAND cp nuttx vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.elf
      COMMAND cp nuttx.hex vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.hex
      COMMAND cp nuttx.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.mapfile
      COMMAND cp System.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.SystemMap
      COMMAND cp nuttx_user vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.elf
      COMMAND cp nuttx_user.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.bin
      COMMAND cp nuttx_user.hex vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.hex
      COMMAND cp nuttx_user.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.mapfile
      COMMAND cp User.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.SystemMap)
else()
  add_custom_target(
    nuttx_post_build
    DEPENDS nuttx systemmap
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND cp nuttx vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.elf
    COMMAND cp nuttx.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.bin
    COMMAND cp nuttx.hex vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.hex
    COMMAND cp nuttx.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.mapfile
    COMMAND cp System.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.SystemMap)
endif()