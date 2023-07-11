#ifndef RPI_EXECUTORS_EXECUTOR_H
#define RPI_EXECUTORS_EXECUTOR_H

#include "messages/Message.h"
#include <memory>


namespace rpi
{
namespace executors
{


class Executor
{
public:
    virtual void run() = 0;
    virtual bool has_result() = 0;
    virtual rpi::messages::MessageRaw get_result() = 0;
};

}
}

#endif