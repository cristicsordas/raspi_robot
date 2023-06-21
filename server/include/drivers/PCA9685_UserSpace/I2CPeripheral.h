#ifndef PIPCA9685_I2CPERIPHERAL_H
#define PIPCA9685_I2CPERIPHERAL_H

#include <cstdint>
#include <string>

// based on the https://github.com/barulicm/PiPCA9685

namespace rpi
{
namespace PiPCA9685US
{

class I2CPeripheral 
{
public:
  I2CPeripheral(const std::string &device = "/dev/i2c-1", uint8_t address = 0x40);
  ~I2CPeripheral();

  void WriteRegisterByte(const uint8_t register_address, const uint8_t value);
  uint8_t ReadRegisterByte(const uint8_t register_address);

private:
  int _bus_fd;

  void OpenBus(const std::string& device);
  void ConnectToPeripheral(const uint8_t address);
};

}  // namespace PiPCA9685US
}  // namespace rpi

#endif  // PIPCA9685_I2CPERIPHERAL_H