#include "socketManager/SocketManager.h"

using rpi::socket_manager::SocketManager;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    SocketManager mgr("9000");
    mgr.start();
}

