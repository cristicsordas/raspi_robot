#include "drivers/PCA9685/PCA9685.h"
#include <unistd.h>
#include <cmath>
#include <iostream>
#include "drivers/PCA9685/Constants.h"
#include <cstring>
#include "../../../../PCA9685_driver/pca_ioctl.h"

using rpi::PiPCA9685::PCA9685;
using namespace rpi::PiPCA9685;

PCA9685::PCA9685()
{
  open();
  set_all_pwm(0, 0);
  write_register_bByte(MODE2, OUTDRV);
  write_register_bByte(MODE1, ALLCALL);
  usleep(5'000);
  auto mode1_val = read_register_byte(MODE1);
  mode1_val &= ~SLEEP;
  write_register_bByte(MODE1, mode1_val);
  usleep(5'000);
}

PCA9685::~PCA9685()
{
  std::cout << "close device" << std::endl;
  fclose(fp_);
}

void PCA9685::set_pwm_freq(const double freq_hz)
{
  frequency = freq_hz;

  auto prescaleval = 2.5e7; //    # 25MHz
  prescaleval /= 4096.0;    //       # 12-bit
  prescaleval /= freq_hz;
  prescaleval -= 1.0;

  uint32_t prescale = static_cast<uint32_t>(std::round(prescaleval));

  if (ioctl(fileno(fp_), PCA_IOC_SET_PWM_FREQV, &prescale))
  {
    std::cout << "PCA_IOC_SET_PWM_FREQV error" << std::endl;
  }
}

void PCA9685::set_pwm(const uint8_t channel, const uint16_t on, const uint16_t off)
{
  struct pca_channel_pwm pwm_channel_value {.channel = channel, .on_value = on, .off_value=off};

  if (ioctl(fileno(fp_), PCA_IOC_SET_PWM_ON_CHANNEL, &pwm_channel_value))
  {
    std::cout << "PCA_IOC_SET_PWM_ON_CHANNEL error" << std::endl;
  }
}

void PCA9685::set_all_pwm(const uint16_t on, const uint16_t off)
{
  struct pca_pwm pwm_value {.on_value = on, .off_value=off};

  if (ioctl(fileno(fp_), PCA_IOC_SET_ALL_PWM, &pwm_value))
  {
    std::cout << "PCA_IOC_SET_ALL_PWM error" << std::endl;
  }
}

void PCA9685::write_register_bByte(const uint8_t register_address, const uint8_t value)
{
  uint8_t buf[2] = {register_address, value}; 
  std::cout << "i2c write addr " << (int)register_address << " value " << (int)value << std::endl;
  fwrite((char*)buf, 1, sizeof(buf), fp_);
  fflush(fp_);
}

uint8_t PCA9685::read_register_byte(const uint8_t register_address)
{
  uint8_t buf[4];
  memset(buf, 0, sizeof(buf));
  std::cout << "read reg addr " << (int)register_address << std::endl;
  size_t bytes_read = fread(buf, 1, sizeof(buf), fp_);
  fflush(fp_);
  std::cout << "reg read nr bytes " <<  (int)bytes_read << " value " <<  (int)buf[0] << std::endl;
  return buf[0] & 0xFF;
}

void PCA9685::open()
{
  fp_ = fopen(device_.c_str(), "a+");
  std::cout << "open device " << device_ << std::endl;
  if (fp_ == NULL)
  {
    std::cout << "open device error";
    throw std::system_error(errno, std::system_category(), "Could not open pca i2c communication");
  }
}
