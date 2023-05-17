#include <iostream>
#include "executors/LedMessageExecutor.h"

using rpi::executors::LedMessageExecutor;

LedMessageExecutor::LedMessageExecutor(rpi::messages::LedMessage& led_message)
{
    led_message_ = led_message;
}


void LedMessageExecutor::run()
{
    if(led_message_.on())
    {
        std::cout << "set led on" << std::endl;
    }
    else if(led_message_.off())
    {
        std::cout << "set led off" << std::endl;
    }
}