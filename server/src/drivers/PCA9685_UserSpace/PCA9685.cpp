#include "drivers/PCA9685_UserSpace/PCA9685.h"
#include <unistd.h>
#include <cmath>
#include "drivers/PCA9685_UserSpace/Constants.h"
#include "drivers/PCA9685_UserSpace/I2CPeripheral.h"

using rpi::PiPCA9685::PCA9685;
using namespace rpi::PiPCA9685;

PCA9685::PCA9685(const std::string &device, int address)
    : _i2c_dev(device, address)
{
  set_all_pwm(0, 0);
  _i2c_dev.WriteRegisterByte(MODE2, OUTDRV);
  _i2c_dev.WriteRegisterByte(MODE1, ALLCALL);
  usleep(5'000);
  auto mode1_val = _i2c_dev.ReadRegisterByte(MODE1);
  mode1_val &= ~SLEEP;
  _i2c_dev.WriteRegisterByte(MODE1, mode1_val);
  usleep(5'000);
}

void PCA9685::set_pwm_freq(const double freq_hz)
{
  frequency = freq_hz;

  auto prescaleval = 2.5e7; //    # 25MHz
  prescaleval /= 4096.0;    //       # 12-bit
  prescaleval /= freq_hz;
  prescaleval -= 1.0;

  auto prescale = static_cast<int>(std::round(prescaleval));

  const auto oldmode = _i2c_dev.ReadRegisterByte(MODE1);

  auto newmode = (oldmode & 0x7F) | SLEEP;

  _i2c_dev.WriteRegisterByte(MODE1, newmode);
  _i2c_dev.WriteRegisterByte(PRESCALE, prescale);
  _i2c_dev.WriteRegisterByte(MODE1, oldmode);
  usleep(5'000);
  _i2c_dev.WriteRegisterByte(MODE1, oldmode | RESTART);
}

void PCA9685::set_pwm(const uint8_t channel, const uint16_t on, const uint16_t off)
{
  const auto channel_offset = 4 * channel;
  _i2c_dev.WriteRegisterByte(LED0_ON_L + channel_offset, on & 0xFF);
  _i2c_dev.WriteRegisterByte(LED0_ON_H + channel_offset, on >> 8);
  _i2c_dev.WriteRegisterByte(LED0_OFF_L + channel_offset, off & 0xFF);
  _i2c_dev.WriteRegisterByte(LED0_OFF_H + channel_offset, off >> 8);
}

void PCA9685::set_all_pwm(const uint16_t on, const uint16_t off)
{
  _i2c_dev.WriteRegisterByte(ALL_LED_ON_L, on & 0xFF);
  _i2c_dev.WriteRegisterByte(ALL_LED_ON_H, on >> 8);
  _i2c_dev.WriteRegisterByte(ALL_LED_OFF_L, off & 0xFF);
  _i2c_dev.WriteRegisterByte(ALL_LED_OFF_H, off >> 8);
}

void PCA9685::set_pwm_ms(const int channel, const double ms)
{
  auto period_ms = 1000.0 / frequency;
  auto bits_per_ms = 4096 / period_ms;
  auto bits = ms * bits_per_ms;
  set_pwm(channel, 0, bits);
}
