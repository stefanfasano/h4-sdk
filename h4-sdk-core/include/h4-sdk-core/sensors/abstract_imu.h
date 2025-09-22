//
// Created by srfas on 9/11/2025.
//

#ifndef H4_SDK_WARMUP_ABSTRACT_IMU_H
#define H4_SDK_WARMUP_ABSTRACT_IMU_H
#include <array>
#include <string>


class abstract_imu
{
  std::string name;

  std::array<float, 3> position; // In world frame (x, y, z)
  std::array<float, 4> quaternion; // In world frame (x, y, z, s)
  std::array<float, 3> angularVelocity; // In sensor frame (x, y, z)
  std::array<float, 3> linear_acceleration; // In sensor frame (x, y, z)

  public:
  explicit abstract_imu(const std::string& name);

  virtual void update();

  virtual void set_position(float x, float y, float z);

  virtual void set_quaternion(float x, float y, float z, float s);

  virtual void set_angular_velocity(float x, float y, float z);

  virtual void set_linear_acceleration(float x, float y, float z);

  virtual std::array<float, 3> get_position();

  virtual std::array<float, 4> get_quaternion();

  virtual std::array<float, 3> get_angular_velocity();

  virtual std::array<float, 3> get_linear_acceleration();

  virtual std::string get_name();

  virtual ~abstract_imu() = default;
};


#endif// H4_SDK_WARMUP_ABSTRACT_IMU_H
