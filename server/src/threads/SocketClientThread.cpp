#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <mutex>

#include "messages/MessageGenerator.h"
#include "socketManager/SocketMgrData.h"
#include "threads/SocketClientThread.h"
#include "aesd_ioctl.h"

using rpi::threads::SocketClientThread;
using rpi::socket_manager::SocketMgrData;
using rpi::messages::MessageGenerator;


SocketClientThread::SocketClientThread(int client)
:client_(client)
{
}

void SocketClientThread::run()
{
    std::cout << "thread started buff" << std::endl;
    FILE *fp = NULL;

    while(isRunning())
    {
        int position = 0;

        char tmp[RECV_BUFF_SIZE];
        int length = recv(client_, &tmp, RECV_BUFF_SIZE, 0);
        if (length != 0)
        {
            memcpy(buffPacket_+position, &tmp, length);
            position += length;
            if(tmp[length - 1] == '\n')
            {
                std::cout << "data received" << std::endl;
                printf("length %d\n", position);
                
                std::unique_ptr<rpi::executors::Executor> executor = MessageGenerator::createMessageExecutor(buffPacket_, position);
                if(executor.get() != nullptr)
                {
                    executor->run();
                }
                position = 0;
            }
        }
        else
        {
            printf("client closed\n");
            stop();
        }
    }

    printf("thread %d stopped\n", client_);
}