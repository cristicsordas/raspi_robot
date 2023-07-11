#ifndef RPI_MESSAGES_MOTORMESSAGE_H
#define RPI_MESSAGES_MOTORMESSAGE_H

#include "messages/Message.h"
#include "messages/MessageIds.h"

namespace rpi
{
namespace messages
{

static const uint8_t MOTOR_MESSAGE_PDU_LENGTH = 10U;

class MotorMessage : public Message<MessageID::TX_MOTOR_MESSAGE, MOTOR_MESSAGE_PDU_LENGTH>
{
public:
    MotorMessage() = default;

    explicit MotorMessage(const MessageRaw& raw_pdu)
    : Message(raw_pdu)
    {}

    enum MOTOR_MESSAGE_ACTION
    {
        MOTOR_MESSAGE_STOP = 0,
        MOTOR_MESSAGE_START = 1,
        MOTOR_MESSAGE_LEFT_UPPER_WHEEL = 2,
        MOTOR_MESSAGE_LEFT_LOWER_WHEEL = 3,
        MOTOR_MESSAGE_RIGHT_UPPER_WHEEL = 4,
        MOTOR_MESSAGE_RIGHT_LOWER_WHEEL = 5,
        MOTOR_MESSAGE_MOVE = 6
    };

    enum Direction
    {
        Forward = 0,
        Backward = 1
    };

    MOTOR_MESSAGE_ACTION get_action() const
    {
        return (static_cast<MOTOR_MESSAGE_ACTION>(pdu_[0]));
    }

    Direction getDirection1() const
    {
        return static_cast<Direction>(pdu_[1]);
    }

    uint32_t get_duty1() const
    {
        return pdu_[2];
    }

    Direction getDirection2() const
    {
        return static_cast<Direction>(pdu_[3]);
    }

    uint32_t get_duty2() const
    {
        return pdu_[4];
    }

    Direction getDirection3() const
    {
        return static_cast<Direction>(pdu_[5]);
    }

    uint32_t get_duty3() const
    {
        return pdu_[6];
    }

    Direction getDirection4() const
    {
        return static_cast<Direction>(pdu_[7]);
    }

    uint32_t get_duty4() const
    {
        return pdu_[8];
    }
};

}
}

#endif