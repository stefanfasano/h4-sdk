//
// Created by srfas on 9/6/2025.
//

#ifndef H4_SDK_WARMUP_H4IMU_H
#define H4_SDK_WARMUP_H4IMU_H
#include "abstract_imu.h"
#include <soem_interface_rsl/EthercatBusBase.hpp>

class h4_imu : public abstract_imu, soem_interface_rsl::EthercatBusBase
{
  public:
  explicit h4_imu(const std::string& name) : abstract_imu(name), EthercatBusBase(name) {
  }

  void update() override
  {

  }
};


#endif// H4_SDK_WARMUP_H4IMU_H
