#include "drivers/PCA9685_UserSpace/I2CPeripheral.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
extern "C"
{
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
  // #include "/usr/include/i2c/smbus.h"
}
#include <system_error>

__s32 i2c_smbus_access(int file, char read_write, __u8 command,
                       int size, union i2c_smbus_data *data)
{
  struct i2c_smbus_ioctl_data args;
  __s32 err;

  args.read_write = read_write;
  args.command = command;
  args.size = size;
  args.data = data;

  err = ioctl(file, I2C_SMBUS, &args);
  if (err == -1)
    err = -errno;
  return err;
}

using rpi::PiPCA9685US::I2CPeripheral;

I2CPeripheral::I2CPeripheral(const std::string &device, const uint8_t address)
{
  OpenBus(device);
  ConnectToPeripheral(address);
}

I2CPeripheral::~I2CPeripheral()
{
  close(_bus_fd);
}

void I2CPeripheral::WriteRegisterByte(const uint8_t register_address, const uint8_t value)
{
  i2c_smbus_data data;
  data.byte = value;
  const auto err = i2c_smbus_access(_bus_fd, I2C_SMBUS_WRITE, register_address, I2C_SMBUS_BYTE_DATA, &data);
  if (err)
  {
    const auto msg = "Could not write value (" + std::to_string(value) + ") to register " + std::to_string(register_address);
    throw std::system_error(errno, std::system_category(), msg);
  }
}

uint8_t I2CPeripheral::ReadRegisterByte(const uint8_t register_address)
{
  i2c_smbus_data data;
  const auto err = i2c_smbus_access(_bus_fd, I2C_SMBUS_READ, register_address, I2C_SMBUS_BYTE_DATA, &data);
  if (err)
  {
    const auto msg = "Could not read value at register " + std::to_string(register_address);
    throw std::system_error(-err, std::system_category(), msg);
  }
  return data.byte & 0xFF;
}

void I2CPeripheral::OpenBus(const std::string &device)
{
  _bus_fd = open(device.c_str(), O_RDWR);
  if (_bus_fd < 0)
  {
    throw std::system_error(errno, std::system_category(), "Could not open i2c bus.");
  }
}

void I2CPeripheral::ConnectToPeripheral(const uint8_t address)
{
  if (ioctl(_bus_fd, I2C_SLAVE, address) < 0)
  {
    throw std::system_error(errno, std::system_category(), "Could not set peripheral address.");
  }
}
