#ifndef RPI_MESSAGES_MESSAGE_H
#define RPI_MESSAGES_MESSAGE_H

#include <stdint.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

#include "messages/MessageIds.h"

namespace rpi
{
namespace messages
{

using MessageRaw = std::vector<uint32_t>;

template<MessageID ID, uint8_t PDU_LENGTH>
class Message
{
public:
    using MessagePDU = std::array<uint32_t,  PDU_LENGTH>;

    Message() = default;

    explicit Message(const MessageRaw &raw_msg)
    {
        std::move(raw_msg.begin() + 1, raw_msg.begin() + PDU_LENGTH, pdu_.begin());
    }

    Message(const Message<ID, PDU_LENGTH> &other)
    {
        if(this != other)
        {
            *this = other;
        }
    }

    Message& operator=(const Message<ID, PDU_LENGTH> &other)
    {
        pdu_ = other.pdu_;
    }
    
    MessageID getID() const
    {
        return ID;
    }

    inline const MessagePDU& pdu() const
    {
        return pdu_;
    } 

protected:
    MessagePDU pdu_;
};

}
}

#endif