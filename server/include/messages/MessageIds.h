#ifndef RPI_MESSAGES_MESSAGE_IDS_H
#define RPI_MESSAGES_MESSAGE_IDS_H

#include <type_traits>

namespace rpi
{
namespace messages
{

enum class MessageID
{
    MOTOR_MESSAGE = 1
};

template <typename Enum>
constexpr typename std::enable_if<std::is_enum<Enum>::value,
                                  typename std::underlying_type<Enum>::type>::type
to_integral(Enum const& value) {
    return static_cast<typename std::underlying_type<Enum>::type>(value);
}

}
}

#endif
