#ifndef RPY_PWM_HAT_PCF8591_H
#define RPY_PWM_HAT_PCF8591_H


#include <string>
#include <memory>

namespace rpi
{
namespace PiPCF8591 
{

class PCF8591
{
public:
    PCF8591();
    ~PCF8591();

    double readADC(uint8_t channel);
    bool isOpened() const
    {
        return (fp_ != NULL);
    }

private:
    uint32_t getValue(uint8_t buf[]);
    const std::string device_{"/dev/pcf8591"};
    FILE *fp_ = NULL;
    void open();
};

}
}

#endif //RPY_PWM_HAT_PCF8591_H
