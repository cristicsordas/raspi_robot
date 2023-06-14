#ifndef RPI_EXECUTORS_MOTOR_MESSAGEEXECUTOR_H
#define RPI_EXECUTORS_MOTOR_MESSAGEEXECUTOR_H

#include "executors/Executor.h"
#include "messages/MotorMessage.h"
#include "drivers/PCA9685_UserSpace/PCA9685.h"
#include <memory>

namespace rpi
{
namespace executors
{

class MotorMessageExecutor : public Executor
{
public:
    MotorMessageExecutor(rpi::messages::MotorMessage& motor_message);

    void run() override;

private:
    void setMotorPwm(uint8_t channel, uint16_t duty);
    void leftUpperWheel(rpi::messages::MotorMessage::Direction direction, uint16_t duty);
    void leftLowerWheel(rpi::messages::MotorMessage::Direction direction, uint16_t duty);
    void rightUpperWheel(rpi::messages::MotorMessage::Direction direction, uint16_t duty);
    void rightLowerWheel(rpi::messages::MotorMessage::Direction direction, uint16_t duty);
    
    void moveCar(rpi::messages::MotorMessage::Direction directionLeftUpper, uint16_t duty1, 
                 rpi::messages::MotorMessage::Direction directionLeftLower, uint16_t duty2, 
                 rpi::messages::MotorMessage::Direction directionrightUpper, uint16_t duty3, 
                 rpi::messages::MotorMessage::Direction directionrightLower, uint16_t duty4);

private:
    static constexpr double PCA_PWM_FREQ_HZ = 50.0;

    rpi::messages::MotorMessage motor_message_;
    rpi::PiPCA9685::PCA9685 _pca{};
};

}
}

#endif