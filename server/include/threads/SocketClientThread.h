#ifndef RPI_SERVER_SOCKET_CLIENT_H
#define RPI_SERVER_SOCKET_CLIENT_H

#include "threads/IThread.h"

namespace rpi
{
namespace threads
{

class SocketClientThread : public IThread<SocketClientThread>
{
public:
    SocketClientThread(int client);

    void run () override;

private:
    static const int PACKET_SIZE = 1024*sizeof(int);
    static const int RECV_BUFF_SIZE = 100;

    uint8_t buffPacket_[PACKET_SIZE];
    int client_ = 0;
};

}
}

#endif