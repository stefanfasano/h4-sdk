# Copyright (2019-2020) Robotics Systems Lab - ETH Zurich:
# Markus Staeuble, Jonas Junger, Johannes Pankert, Philipp Leemann,
# Tom Lankhorst, Samuel Bachmann, Gabriel Hottiger, Lennert Nachtigall,
# Mario Mauerer, Remo Diethelm
#
# This file is part of the soem_interface_rsl.
#
# The soem_interface_rsl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The seom_interface is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the soem_interface_rsl.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.14)
project(h4-sdk-core LANGUAGES CXX)

# Build settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(${PROJECT_NAME}
  src/${PROJECT_NAME}/sensors/abstract_imu.cpp
  src/${PROJECT_NAME}/sensors/abstract_motor_controller.cpp
  src/${PROJECT_NAME}/sensors/h4_ethersnacks_board.cpp
  src/${PROJECT_NAME}/sensors/h4_imu.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
target_compile_definitions(${PROJECT_NAME} PRIVATE MELO_USE_COUT)

# Public headers for this target and build-time include of sibling dependency headers
# Note: soem_interface_rsl should be added as a sibling subdirectory in the top-level CMakeLists.txt
# before this target, so that its headers and library are available.
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    # Needed because public headers include <soem_interface_rsl/...>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../soem_interface_rsl/include>
)

# Link against the sibling library
# The soem_interface_rsl target is expected to be defined in ../soem_interface_rsl
# and linked to its own dependencies.
target_link_libraries(${PROJECT_NAME}
  PRIVATE soem_interface_rsl
)

# Example executable
#add_executable(${PROJECT_NAME}_1
#  src/example_slave.cpp
#)
#
#target_compile_options(${PROJECT_NAME}_1 PRIVATE -Wall -Werror)
#
#target_link_libraries(${PROJECT_NAME}_1
#  PRIVATE ${PROJECT_NAME}
#)
#
## Install rules (standard locations)
#install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_1
#  ARCHIVE DESTINATION lib
#  LIBRARY DESTINATION lib
#  RUNTIME DESTINATION bin
#)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)
