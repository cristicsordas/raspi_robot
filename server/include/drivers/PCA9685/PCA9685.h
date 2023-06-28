#ifndef RPY_PWM_HAT_PCA9685_H
#define RPY_PWM_HAT_PCA9685_H

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
    PCA9685();
    ~PCA9685();

    void set_pwm_freq(const double freq_hz);
    void set_pwm(const uint8_t channel, const uint16_t on, const uint16_t off);
    void set_all_pwm(const uint16_t on, const uint16_t off);

private:
  void write_register_bByte(const uint8_t register_address, const uint8_t value);
  uint8_t read_register_byte(const uint8_t register_address);

private:
    const std::string device_{"/dev/pca9685"};
    FILE *fp_ =NULL;
    void open();
    

    // Default frequency pulled from PCA9685 datasheet.
    double frequency = 200.0;
};

}  // namespace PiPCA9685
}  // namespace rpi

#endif //RPY_PWM_HAT_PCA9685_H
