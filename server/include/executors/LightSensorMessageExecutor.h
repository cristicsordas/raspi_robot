#ifndef RPI_EXECUTORS_LIGHT_MESSAGEEXECUTOR_H
#define RPI_EXECUTORS_LIGHT_MESSAGEEXECUTOR_H

#include "executors/Executor.h"
#include "messages/TxLightSensorMessage.h"
#include "drivers/PCF8591/PCF8591.h"
#include <memory>

namespace rpi
{
namespace executors
{

class LightSensorMessageExecutor : public Executor
{
public:
    LightSensorMessageExecutor(rpi::messages::TxLightSensorMessage& tx_light_sensor_message);

    void run() override;

    bool has_result() override
    {
        return has_result_;
    }

    std::vector<uint8_t> get_result() override
    {
        return result_;
    }

private:
    void fill_raw_result(uint32_t value);

private:
    rpi::messages::TxLightSensorMessage tx_light_sensor_message_;
    rpi::PiPCF8591::PCF8591 pcf_{};
    bool has_result_ = false;
    std::vector<uint8_t> result_;
};

}
}

#endif