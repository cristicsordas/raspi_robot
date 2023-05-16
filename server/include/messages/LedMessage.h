#ifndef RPI_MESSAGES_LEDMESSAGE_H
#define RPI_MESSAGES_LEDMESSAGE_H

#include "messages/Message.h"
#include "messages/MessageIds.h"

namespace rpi
{
namespace messages
{

static const uint8_t LED_MESSAGE_PDU_LENGTH = 2U;

class LedMessage : public Message<MessageID::LED_MESSAGE_ID, LED_MESSAGE_PDU_LENGTH>
{
public:
    LedMessage() = default;

    explicit LedMessage(const MessageRaw& raw_pdu)
    : Message(raw_pdu)
    {}

    bool isOn() const
    {
        return (pdu_[0] == 1);
    }

    bool isOff() const
    {
        return (pdu_[0] == 0);
    }
};

}
}

#endif