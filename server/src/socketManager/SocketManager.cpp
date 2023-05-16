#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <memory>

#include "socketManager/SocketManager.h"
#include "socketManager/SocketMgrData.h"
#include "threads/SocketClientThread.h"


using rpi::socket_manager::SocketMgrData;
using rpi::threads::IBaseThread;
using rpi::threads::SocketClientThread;

namespace rpi
{
namespace socket_manager
{

SocketManager::SocketManager(const std::string &&port)
{
    port_ = std::move(port);
}

void SocketManager::configure(bool daemon)
{
    memset(&hints_, 0, sizeof(hints_));

    hints_.ai_family = AF_INET;
    hints_.ai_socktype = SOCK_STREAM;
    hints_.ai_flags = AI_PASSIVE;
    hints_.ai_protocol = 0;

    listenfd_ = 0;
    daemon_ = daemon;
}

bool SocketManager::startListening()
{
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd_ == -1)
    {
        std::cout << "cannot create socket" << std::endl;
        return false;
    }

    struct addrinfo *ret_addr_info;

    int info_res = getaddrinfo(NULL, port_.c_str(), &hints_, &ret_addr_info);
    if (info_res != 0) 
    {
        std::cout << "Error:" << stderr << "getaddrinfo:" << gai_strerror(info_res) << std::endl;
        return -1;
    }    

    if(bind(listenfd_, ret_addr_info->ai_addr, ret_addr_info->ai_addrlen) != 0)
    {
        std::cout << "bind error" << std::endl;
        return false;
    }

    freeaddrinfo(ret_addr_info);

    if(!startDaemon())
    {
        return false;
    }

    if(listen(listenfd_, 100) != 0)
    {
        std::cout << "server listen error" << std::endl;
        return false;
    }


    return true;
}

bool SocketManager::startDaemon()
{
    if(daemon_)
    {
        std::cout << "fork" << std::endl;
        pid_t pid = fork();
        if(pid == 0)
        {
            std::cout << "I am the child" << std::endl;
        }
        else if(pid > 0)
        {
            std::cout << "I am the parent" << std::endl;
            exit(0);
        }
        else
        {
            printf("Error in fork\n");
            return false;
        }
    }
    return true;
}

void SocketManager::sigActionHandler(int sig, siginfo_t *info, void *context)
{
    if((sig == SIGINT) || (sig == SIGTERM))
    {
        SocketMgrData::getInstance().setRunning(false);
        std::cout << "stop server" << std::endl;
    }
}

void SocketManager::setSigActions()
{
    struct sigaction act_int = { 0 };
    act_int.sa_sigaction = &SocketManager::sigActionHandler;
    if (sigaction(SIGINT, &act_int, NULL) == -1) 
    {
        std::cout << "set sigint error" << std::endl;
    }
    struct sigaction act_term = { 0 };
    act_term.sa_sigaction = &SocketManager::sigActionHandler;
    if (sigaction(SIGTERM, &act_term, NULL) == -1) 
    {
        std::cout << "set sigterm error" << std::endl;
    }
}

void SocketManager::run()
{
    SocketMgrData::getInstance().setRunning(true);
    while(SocketMgrData::getInstance().isRunning())
    {
        struct sockaddr_in addr_client;
        socklen_t client_length = 0;
        int client = accept(listenfd_, (struct sockaddr*)&addr_client, &client_length);
        if(client > 0)
        {
            std::cout << "Accepted connection from :" 
                      << (addr_client.sin_addr.s_addr&0xFF) << ", "
                      << ((addr_client.sin_addr.s_addr&0xFF00)>>8) << ", "
                      << ((addr_client.sin_addr.s_addr&0xFF0000)>>16) << ", " 
                      << ((addr_client.sin_addr.s_addr&0xFF000000)>>24) << std::endl;

            std::shared_ptr<IBaseThread> ptrClientThread = std::make_shared<SocketClientThread>(client);
            ptrClientThread->start();
            SocketMgrData::getInstance().addThread(ptrClientThread);
            SocketMgrData::getInstance().removeCompletedThreads();
        }
        std::cout << "server cyclic" << std::endl;
    }
}

void SocketManager::start(bool daemon)
{
    configure(daemon);
    if(startListening())
    {
        setSigActions();
        run();
    }
    SocketMgrData::getInstance().stopAllThreads();
}




}
}