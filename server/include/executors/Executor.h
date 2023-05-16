#ifndef RPI_EXECUTORS_EXECUTOR_H
#define RPI_EXECUTORS_EXECUTOR_H

namespace rpi
{
namespace executors
{


class Executor
{
public:
    virtual void run() = 0;
};

}
}

#endif