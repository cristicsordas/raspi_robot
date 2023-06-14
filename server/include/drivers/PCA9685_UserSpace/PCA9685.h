#ifndef RPY_PWM_HAT_PCA9685_H
#define RPY_PWM_HAT_PCA9685_H

#include "drivers/PCA9685_UserSpace/I2CPeripheral.h"
#include <string>
#include <memory>

// based on the https://github.com/barulicm/PiPCA9685

namespace rpi
{
namespace PiPCA9685 
{

class PCA9685
{
public:
    explicit PCA9685(const std::string &device = "/dev/i2c-1", int address = 0x40);

    void set_pwm_freq(const double freq_hz);
    void set_pwm(const uint8_t channel, const uint16_t on, const uint16_t off);
    void set_all_pwm(const uint16_t on, const uint16_t off);
    void set_pwm_ms(const int channel, const double ms);

private:
    I2CPeripheral _i2c_dev;
    
    // Default frequency pulled from PCA9685 datasheet.
    double frequency = 200.0;

};

}  // namespace PiPCA9685
}  // namespace rpi

#endif //RPY_PWM_HAT_PCA9685_H
