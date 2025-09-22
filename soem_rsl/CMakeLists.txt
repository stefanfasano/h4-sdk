cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
project(soem_rsl LANGUAGES C)

# Option to build tests (enabled by default to preserve previous behavior)
option(BUILD_TESTS "Build SOEM test utilities" ON)

# Detect platform and set flags/libs
if(WIN32)
  set(OS "win32")
  include_directories(oshw/win32/wpcap/Include)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/oshw/win32/wpcap/Lib/x64)
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/oshw/win32/wpcap/Lib)
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_NO_WARNINGS")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
  set(OS_LIBS wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
elseif(UNIX AND NOT APPLE)
  set(OS "linux")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
  set(OS_LIBS pthread rt)
elseif(APPLE)
  # This must come before linux or MacOSX will identify as Unix.
  set(OS "macosx")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
  set(OS_LIBS pthread pcap)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "rt-kernel")
  set(OS "rtk")
  message(STATUS "ARCH is ${ARCH}")
  message(STATUS "BSP is ${BSP}")
  include_directories(oshw/${OS}/${ARCH})
  file(GLOB OSHW_EXTRA_SOURCES oshw/${OS}/${ARCH}/*.c)
  # Keep original flags from catkin version
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format")
  set(OS_LIBS "-Wl,--start-group -l${BSP} -l${ARCH} -lkern -ldev -lsio -lblock -lfs -lusb -llwip -leth -li2c -lrtc -lcan -lnand -lspi -lnor -lpwm -ladc -ltrace -lc -lm -Wl,--end-group")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "rtems")
  message(STATUS "Building for RTEMS")
  set(OS "rtems")
  set(soem_rsl_LIB_INSTALL_DIR ${LIB_DIR})
  set(BUILD_TESTS FALSE)
endif()

message(STATUS "OS is ${OS}")

# Install destinations (plain CMake defaults)
set(soem_rsl_INCLUDE_INSTALL_DIR include/soem_rsl)
set(soem_rsl_LIB_INSTALL_DIR ${soem_rsl_LIB_INSTALL_DIR})
if(NOT soem_rsl_LIB_INSTALL_DIR)
  set(soem_rsl_LIB_INSTALL_DIR lib)
endif()

# Sources and headers
file(GLOB soem_rsl_SOURCES soem_rsl/soem_rsl/*.c)
file(GLOB OSAL_SOURCES soem_rsl/osal/${OS}/*.c)
file(GLOB OSHW_SOURCES soem_rsl/oshw/${OS}/*.c)

file(GLOB soem_rsl_HEADERS soem_rsl/soem_rsl/*.h)
file(GLOB OSAL_HEADERS soem_rsl/osal/osal.h soem_rsl/osal/${OS}/*.h)
file(GLOB OSHW_HEADERS soem_rsl/oshw/${OS}/*.h)

# Library target
add_library(soem_rsl STATIC
  ${soem_rsl_SOURCES}
  ${OSAL_SOURCES}
  ${OSHW_SOURCES}
  ${OSHW_EXTRA_SOURCES}
)

target_link_libraries(soem_rsl ${OS_LIBS})
set_target_properties(soem_rsl PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Public include directories for consumers
# Build tree includes match source layout; install tree installs headers into include/soem_rsl

target_include_directories(soem_rsl PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/soem_rsl/soem_rsl>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/soem_rsl/osal>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/soem_rsl/osal/${OS}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/soem_rsl/oshw/${OS}>
  $<INSTALL_INTERFACE:include/soem_rsl>
)

message(STATUS "LIB_DIR: ${soem_rsl_LIB_INSTALL_DIR}")

# Install: library, export config, and headers
install(TARGETS soem_rsl EXPORT soem_rslConfig DESTINATION ${soem_rsl_LIB_INSTALL_DIR})
install(EXPORT soem_rslConfig DESTINATION share/soem_rsl/cmake)

install(FILES
  ${soem_rsl_HEADERS}
  ${OSAL_HEADERS}
  ${OSHW_HEADERS}
  DESTINATION ${soem_rsl_INCLUDE_INSTALL_DIR}
)

# For compatibility some consumers expect ethercattype.h directly in include/
install(FILES
  ./soem_rsl/soem_rsl/ethercattype.h
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

# Tests (Linux paths; retained from original)
if(BUILD_TESTS)
  add_subdirectory(soem_rsl/test/linux/slaveinfo)
  add_subdirectory(soem_rsl/test/linux/eepromtool)
  add_subdirectory(soem_rsl/test/linux/simple_test)
endif()
