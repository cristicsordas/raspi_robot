#include <vector>
#include <sstream>
#include "messages/MessageGenerator.h"
#include "messages/MessageIds.h"
#include "messages/LedMessage.h"
#include "executors/LedMessageExecutor.h"

using rpi::messages::MessageGenerator;
using rpi::messages::MessageID;
using rpi::messages::LedMessage;
using rpi::executors::LedMessageExecutor;

std::vector<uint32_t> MessageGenerator::splitRawMsg(uint8_t raw_msg[], uint8_t length)
{
    std::vector<uint32_t> results;
    if(raw_msg != nullptr)
    {
        std::string recvString(reinterpret_cast<const char *>(raw_msg), length);
        std::stringstream ss(recvString);

        std::string str_split;
        while(getline(ss, str_split, ' '))
        {
            uint32_t number;
            std::istringstream(str_split) >> number;
            results.push_back(number);
        }
    }
    return results;
}

std::unique_ptr<rpi::executors::Executor> MessageGenerator::createMessageExecutor(uint8_t raw_msg[], uint8_t length)
{
    std::unique_ptr<rpi::executors::Executor> executor;
    std::vector<uint32_t> results = splitRawMsg(raw_msg, length);

    if(results.size() != 0)
    {
        MessageID msg_id = static_cast<MessageID>(results[0]);
        switch (msg_id)
        {
            case MessageID::LED_MESSAGE_ID:
            {
                LedMessage led_msg(results);
                executor = std::make_unique<LedMessageExecutor>(led_msg);
            } break;
        
        default:
            break;
        }
    }

    return executor;
}

