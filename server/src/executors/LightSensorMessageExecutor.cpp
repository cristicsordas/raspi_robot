#include <iostream>
#include <cmath>
#include "executors/LightSensorMessageExecutor.h"

using rpi::executors::LightSensorMessageExecutor;
using rpi::messages::TxLightSensorMessage;

LightSensorMessageExecutor::LightSensorMessageExecutor(rpi::messages::TxLightSensorMessage& tx_light_sensor_message)
{
    tx_light_sensor_message_ = tx_light_sensor_message;
}

void LightSensorMessageExecutor::fill_raw_result(uint32_t value)
{
    result_.push_back(to_integral(rpi::messages::MessageID::RX_LIGHT_SENSOR));
    result_.push_back(static_cast<uint8_t>(value >> 8));
    result_.push_back(static_cast<uint8_t>(value & 0xFF));
    std::cout << "result " << (int)result_[1] << " " << (int)result_[2] << std::endl;
}

void LightSensorMessageExecutor::run()
{
    try
    {
        if(pcf_.isOpened())
        {
            std::cout << "pcf channel " << tx_light_sensor_message_.get_channel() << std::endl;
            uint32_t voltage = pcf_.readADC(tx_light_sensor_message_.get_channel());
            has_result_ = true;
            fill_raw_result(voltage);
        }
    }
    catch(const std::system_error& e)
    {
        has_result_ = false;
        std::cout << "System Error " << e.what() << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        has_result_ = false;
        std::cout << "Runtime Error " << e.what() << std::endl;
    }
    catch(...)
    {
        has_result_ = false;
        std::cout << "Error " << std::endl;
    }
 }


