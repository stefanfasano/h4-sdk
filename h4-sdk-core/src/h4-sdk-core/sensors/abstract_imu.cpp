//
// Created by srfas on 9/12/2025.
//

#include "h4-sdk-core/sensors/abstract_imu.h"
#include <array>
#include <string>


abstract_imu::abstract_imu(const std::string& name) : name(name), position(), quaternion(), angularVelocity(), linear_acceleration() {}

void abstract_imu::update() {  }

void abstract_imu::set_position(float x, float y, float z)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
}

void abstract_imu::set_quaternion(float x, float y, float z, float s)
{
  quaternion[0] = x;
  quaternion[1] = y;
  quaternion[2] = z;
  quaternion[3] = s;
}

void abstract_imu::set_angular_velocity(float x, float y, float z)
{
  angularVelocity[0] = x;
  angularVelocity[1] = y;
  angularVelocity[2] = z;
}

void abstract_imu::set_linear_acceleration(float x, float y, float z)
{
  linear_acceleration[0] = x;
  linear_acceleration[1] = y;
  linear_acceleration[2] = z;
}

std::array<float, 3> abstract_imu::get_position()
{
  return position;
}

std::array<float, 4> abstract_imu::get_quaternion()
{
  return quaternion;
}

std::array<float, 3> abstract_imu::get_angular_velocity()
{
  return angularVelocity;
}

std::array<float, 3> abstract_imu::get_linear_acceleration()
{
  return linear_acceleration;
}

std::string abstract_imu::get_name()
{
  return name;
}