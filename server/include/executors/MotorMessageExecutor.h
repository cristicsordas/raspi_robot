#ifndef RPI_EXECUTORS_MOTOR_MESSAGEEXECUTOR_H
#define RPI_EXECUTORS_MOTOR_MESSAGEEXECUTOR_H

#include "executors/Executor.h"
#include "messages/MotorMessage.h"
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
    rpi::messages::MotorMessage motor_message_;
};

}
}

#endif