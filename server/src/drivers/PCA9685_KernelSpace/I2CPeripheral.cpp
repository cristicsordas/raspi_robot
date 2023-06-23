#include "drivers/PCA9685_KernelSpace/I2CPeripheral.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>

extern "C"
{
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
  // #include "/usr/include/i2c/smbus.h"
}
#include <system_error>

using rpi::PiPCA9685KS::I2CPeripheral;

I2CPeripheral::I2CPeripheral(const std::string &device)
{
  OpenBus(device);
}

I2CPeripheral::~I2CPeripheral()
{
  std::cout << "close device" << std::endl;
  fclose(fp);
}

void I2CPeripheral::WriteRegisterByte(const uint8_t register_address, const uint8_t value)
{
  uint8_t buf[2] = {register_address, value}; 
  std::cout << "i2c write addr " << (int)register_address << " value " << (int)value << std::endl;
  fwrite((char*)buf, 1, sizeof(buf), fp);
}

uint8_t I2CPeripheral::ReadRegisterByte(const uint8_t register_address)
{
  uint8_t buf[2]; 
  std::cout << "read reg addr " << (int)register_address << std::endl;
  size_t bytes_read = fread(buf, 1, sizeof(buf), fp);
  std::cout << "reg read" << std::endl;
  if (bytes_read == 0)
  {
    const auto msg = "Could not read value at register " + std::to_string(register_address);
    throw std::system_error(errno, std::system_category(), msg);
  }
  return buf[0] & 0xFF;
}

void I2CPeripheral::OpenBus(const std::string &device)
{
  fp = fopen(device.c_str(), "a+");
  std::cout << "open device " << device << std::endl;
  if(fp == nullptr)
  {
    std::cout << "open device error";
    throw std::system_error(errno, std::system_category(), "Could not open pca i2c communication");
  }
}

