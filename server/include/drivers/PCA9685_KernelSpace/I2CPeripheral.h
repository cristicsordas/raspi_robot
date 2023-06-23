#ifndef PIPCA9685_I2CPERIPHERAL_KS_H
#define PIPCA9685_I2CPERIPHERAL_KS_H

#include <cstdint>
#include <string>

// based on the https://github.com/barulicm/PiPCA9685

namespace rpi
{
namespace PiPCA9685KS
{

class I2CPeripheral
{
public:
  I2CPeripheral(const std::string &device = "/dev/pca9685");
  ~I2CPeripheral();

  void WriteRegisterByte(const uint8_t register_address, const uint8_t value);
  uint8_t ReadRegisterByte(const uint8_t register_address);

private:
  FILE *fp =NULL;

  void OpenBus(const std::string& device);
};

}  // namespace PiPCA9685KS
}  // namespace rpi

#endif  // PIPCA9685_I2CPERIPHERAL_KS_H