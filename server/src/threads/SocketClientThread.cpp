#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <mutex>

#include "messages/MessageGenerator.h"
#include "socketManager/SocketMgrData.h"
#include "threads/SocketClientThread.h"

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

    while(isRunning())
    {
        int total_length = 0;

        char tmp[RECV_BUFF_SIZE];
        int length = recv(client_, &tmp, RECV_BUFF_SIZE, 0);
        if (length != 0)
        {
            memcpy(buffPacket_+total_length, &tmp, length);
            total_length += length;
            if(tmp[length - 1] == '\n')
            {
                std::cout << "data received. length: " << total_length << std::endl;
                
                std::unique_ptr<rpi::executors::Executor> executor = MessageGenerator::createMessageExecutor(buffPacket_, total_length);
                if(executor.get() != nullptr)
                {
                    executor->run();
                }
                total_length = 0;
            }
        }
        else
        {
            std::cout << "client closed" << std::endl;
            stop();
        }
    }

    std::cout << "thread: " << client_ <<  "stopped" << std::endl;
}