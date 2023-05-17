#ifndef RPI_MESSAGES_MOTORMESSAGE_H
#define RPI_MESSAGES_MOTORMESSAGE_H

#include "messages/Message.h"
#include "messages/MessageIds.h"

namespace rpi
{
namespace messages
{

static const uint8_t MOTOR_MESSAGE_PDU_LENGTH = 2U;

class MotorMessage : public Message<MessageID::LED_MESSAGE_ID, MOTOR_MESSAGE_PDU_LENGTH>
{
public:
    MotorMessage() = default;

    explicit MotorMessage(const MessageRaw& raw_pdu)
    : Message(raw_pdu)
    {}

    bool start() const
    {
        return (static_cast<bool>(pdu_[0]) == true);
    }

    bool stop() const
    {
        return (static_cast<bool>(pdu_[0]) == false);
    }
};

}
}

#endif