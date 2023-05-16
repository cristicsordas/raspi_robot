#ifndef RPI_EXECUTORS_LEDMESSAGEEXECUTOR_H
#define RPI_EXECUTORS_LEDMESSAGEEXECUTOR_H

#include "executors/Executor.h"
#include "messages/LedMessage.h"
#include <memory>

namespace rpi
{
namespace executors
{

class LedMessageExecutor : public Executor
{
public:
    LedMessageExecutor(rpi::messages::LedMessage& led_message);


    void run() override;

private:
    rpi::messages::LedMessage led_message_;
};

}
}

#endif