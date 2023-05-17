#ifndef RPI_SERVER_SOCKETMANAGER_H
#define RPI_SERVER_SOCKETMANAGER_H

#include <string>
#include <netdb.h>
#include <signal.h>


namespace rpi
{
namespace socket_manager
{

class SocketManager
{
public:
    SocketManager(const std::string &&port = DEFAULT_PORT);

    void start(bool daemon = false);
    bool isDaemon() const { return daemon_; }

private:
    void run();
    void configure(bool daemon);
    bool startListening();
    bool startDaemon();
    void setSigActions();

    static void sigActionHandler(int sig, siginfo_t *info, void *context);

private:
    static constexpr char const* DEFAULT_PORT{"9000"};

    std::string port_ = DEFAULT_PORT;
    bool daemon_ = false;
    int listenfd_ = 0;

    struct addrinfo hints_;
};

}
}

#endif