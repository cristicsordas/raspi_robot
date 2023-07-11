#include <iostream>
#include <cmath>
#include "executors/LightSensorMessageExecutor.h"

using rpi::executors::LightSensorMessageExecutor;
using rpi::messages::TxLightSensorMessage;

LightSensorMessageExecutor::LightSensorMessageExecutor(rpi::messages::TxLightSensorMessage& tx_light_sensor_message)
{
    tx_light_sensor_message_ = tx_light_sensor_message;
}

void LightSensorMessageExecutor::fill_raw_result(double value)
{
    float whole, fractional;
    fractional = std::modf(value, &whole);

    result_.push_back(to_integral(rpi::messages::MessageID::RX_LIGHT_SENSOR));
    result_.push_back(static_cast<uint8_t>(whole));
    result_.push_back(static_cast<uint8_t>(fractional*100));
}

void LightSensorMessageExecutor::run()
{
    try
    {
        if(pcf_.isOpened())
        {
            std::cout << "pcf channel " << tx_light_sensor_message_.get_channel() << std::endl;
            double voltage = pcf_.readADC(tx_light_sensor_message_.get_channel());
            if(voltage != 0.0)
            {
                has_result_ = true;
                fill_raw_result(voltage);
            }
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


