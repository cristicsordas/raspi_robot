#include <iostream>
#include "executors/MotorMessageExecutor.h"

using rpi::executors::MotorMessageExecutor;

MotorMessageExecutor::MotorMessageExecutor(rpi::messages::MotorMessage& motor_message)
{
    motor_message_ = motor_message;
}


void MotorMessageExecutor::run()
{
    if(motor_message_.start())
    {
        std::cout << "move the car" << std::endl;
    }
    else if(motor_message_.stop())
    {
        std::cout << "stop the car" << std::endl;
    }
}