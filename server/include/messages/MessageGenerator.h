#ifndef RPI_MESSAGES_MESSAGEGENERATOR_H
#define RPI_MESSAGES_MESSAGEGENERATOR_H

#include <vector>
#include <memory>
#include "executors/Executor.h"

namespace rpi
{
namespace messages
{

class MessageGenerator
{
public:
    static std::unique_ptr<rpi::executors::Executor> createMessageExecutor(uint8_t raw_msg[], uint8_t length);

private:
    static std::vector<uint32_t> splitRawMsg(uint8_t raw_msg[], uint8_t length);
};

}
}

#endif