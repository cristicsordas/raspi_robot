#ifndef RPI_EXECUTORS_EXECUTOR_H
#define RPI_EXECUTORS_EXECUTOR_H

#include <memory>
#include <vector>

namespace rpi
{
namespace executors
{


class Executor
{
public:
    virtual void run() = 0;
    virtual bool has_result() = 0;
    virtual std::vector<uint8_t> get_result() = 0;
};

}
}

#endif