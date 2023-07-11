#ifndef RPY_PWM_HAT_PCF8591_H
#define RPY_PWM_HAT_PCF8591_H


#include <string>
#include <memory>

// based on the https://github.com/barulicm/PiPCA9685

namespace rpi
{
namespace PiPCF8591 
{

class PCA9685
{
public:
    PCA9685();
    ~PCA9685();

    double readADC(uint8_t channel);

private:
    uint32_t getValue(uint8_t buf[]);
    const std::string device_{"/dev/pcf8591"};
    FILE *fp_ = NULL;
    void open();
};

}
}

#endif //RPY_PWM_HAT_PCF8591_H
