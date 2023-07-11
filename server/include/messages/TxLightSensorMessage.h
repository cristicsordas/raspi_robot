#ifndef RPI_MESSAGES_LIGHT_SENSOR_ESSAGE_H
#define RPI_MESSAGES_LIGHT_SENSOR_ESSAGE_H

#include "messages/Message.h"
#include "messages/MessageIds.h"
#include "drivers/PCF8591/PCF8591.h"

namespace rpi
{
namespace messages
{

static const uint8_t TX_LIGHT_SENSOR_PDU_LENGTH = 2U;

class TxLightSensorMessage : public Message<MessageID::TX_LIGHT_SENSOR, TX_LIGHT_SENSOR_PDU_LENGTH>
{
public:
    TxLightSensorMessage() = default;

    explicit TxLightSensorMessage(const MessageRaw& raw_pdu)
    : Message(raw_pdu)
    {}

    uint8_t get_channel() const
    {
        return static_cast<uint8_t>(pdu_[0]);
    }
};

}
}

#endif
