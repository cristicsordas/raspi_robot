#include <iostream>
#include "executors/MotorMessageExecutor.h"

using rpi::executors::MotorMessageExecutor;
using rpi::messages::MotorMessage;

MotorMessageExecutor::MotorMessageExecutor(rpi::messages::MotorMessage& motor_message)
{
    motor_message_ = motor_message;
    _pca.set_pwm_freq(PCA_PWM_FREQ_HZ);
}

void MotorMessageExecutor::setMotorPwm(uint8_t channel, uint16_t duty)
{
    _pca.set_pwm(channel, 0, duty);
}

void MotorMessageExecutor::setWheelPwm(uint8_t channel, rpi::messages::MotorMessage::Direction direction, uint16_t duty)
{
    std::cout << "set pwm " << "channel " << channel << " direction " << direction << " duty " << duty << std::endl;
    if(direction == MotorMessage::Backward)
    {
        setMotorPwm(channel, 0);
        setMotorPwm(channel + 1, duty);
    }
    else if(direction == MotorMessage::Forward)
    {
        setMotorPwm(channel + 1, 0);
        setMotorPwm(channel, duty);
    }
}

void MotorMessageExecutor::stopWheelPwm(uint8_t channel)
{
    setMotorPwm(channel, 0x0FFFU);
    setMotorPwm(channel + 1, 0x0FFFU);
}

void MotorMessageExecutor::leftUpperWheel(MotorMessage::Direction direction, uint16_t duty)
{
    if(duty == 0)
    {
        stopWheelPwm(CHANNEL_LEFT_UPPER);
        return;
    }
    setWheelPwm(CHANNEL_LEFT_UPPER, direction, duty);
}

void MotorMessageExecutor::leftLowerWheel(MotorMessage::Direction direction, uint16_t duty)
{
    if(duty == 0)
    {
        stopWheelPwm(CHANNEL_LEFT_LOWER);
        return;
    }

    direction = (direction == MotorMessage::Backward) ? MotorMessage::Forward : MotorMessage::Backward;

    setWheelPwm(CHANNEL_LEFT_LOWER, direction, duty);
}

void MotorMessageExecutor::rightUpperWheel(MotorMessage::Direction direction, uint16_t duty)
{
    if(duty == 0)
    {
        stopWheelPwm(CHANNEL_RIGHT_UPPER);
        return;
    }

    setWheelPwm(CHANNEL_RIGHT_UPPER, direction, duty);
}

void MotorMessageExecutor::rightLowerWheel(MotorMessage::Direction direction, uint16_t duty)
{
    if(duty == 0)
    {
        stopWheelPwm(CHANNEL_RIGHT_LOWER);
    }

    setWheelPwm(CHANNEL_RIGHT_LOWER, direction, duty);
}

void MotorMessageExecutor::moveCar(MotorMessage::Direction directionLeftUpper, uint16_t duty1, MotorMessage::Direction directionLeftLower, uint16_t duty2, 
            MotorMessage::Direction directionrightUpper, uint16_t duty3, MotorMessage::Direction directionrightLower, uint16_t duty4)
{
    leftUpperWheel(directionLeftUpper, duty1);
    leftLowerWheel(directionLeftLower, duty2);
    rightUpperWheel(directionrightUpper, duty3);
    rightLowerWheel(directionrightLower, duty4);
}

void MotorMessageExecutor::run()
{
    try
    {
        switch(motor_message_.get_action())
        {
            case MotorMessage::MOTOR_MESSAGE_STOP:
            {
                std::cout << "stop the car" << std::endl;
                moveCar(MotorMessage::Forward, 0U, 
                        MotorMessage::Forward, 0U, 
                        MotorMessage::Forward, 0U, 
                        MotorMessage::Forward, 0U);
            } break;
            case MotorMessage::MOTOR_MESSAGE_START:
            {
                std::cout << "move the car" << std::endl;
                moveCar(MotorMessage::Forward, 2000U, 
                        MotorMessage::Forward, 2000U, 
                        MotorMessage::Forward, 2000U, 
                        MotorMessage::Forward, 2000U);
            } break;
            case MotorMessage::MOTOR_MESSAGE_LEFT_UPPER_WHEEL:
            {
                std::cout << "left upper wheel " << "direction: " << motor_message_.getDirection1() << std::endl;
                leftUpperWheel(motor_message_.getDirection1(), motor_message_.get_duty1());
            } break;
            case MotorMessage::MOTOR_MESSAGE_LEFT_LOWER_WHEEL:
            {
                std::cout << "left lower wheel" << std::endl;
                leftLowerWheel(motor_message_.getDirection1(), motor_message_.get_duty1());
            } break;
            case MotorMessage::MOTOR_MESSAGE_RIGHT_UPPER_WHEEL:
            {
                std::cout << "right upper wheel" << std::endl;
                rightUpperWheel(motor_message_.getDirection1(), motor_message_.get_duty1());
            } break;
            case MotorMessage::MOTOR_MESSAGE_RIGHT_LOWER_WHEEL:
            {
                std::cout << "right lower wheel" << std::endl;
                rightLowerWheel(motor_message_.getDirection1(), motor_message_.get_duty1());
            } break;
            case MotorMessage::MOTOR_MESSAGE_MOVE:
            {
                std::cout << "motor model" << std::endl;
                std::cout << "duty " << motor_message_.get_duty1() << " " << motor_message_.get_duty2() << " " << motor_message_.get_duty3() << " " << motor_message_.get_duty4() << std::endl;
                moveCar(motor_message_.getDirection1(), motor_message_.get_duty1(), 
                        motor_message_.getDirection2(), motor_message_.get_duty2(), 
                        motor_message_.getDirection3(), motor_message_.get_duty3(), 
                        motor_message_.getDirection4(), motor_message_.get_duty4());
            } break;
        }
    }
    catch(const std::system_error& e)
    {
        std::cout << "System Error " << e.what() << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Runtime Error " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Error " << std::endl;
    }
   
}