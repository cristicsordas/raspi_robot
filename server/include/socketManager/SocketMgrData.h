#include <mutex>
#include <vector>
#include <memory>
#include "threads/IThread.h"

namespace rpi
{
namespace socket_manager
{

//shared data used by the socket manager
class SocketMgrData
{
public:
    static SocketMgrData& getInstance();

    SocketMgrData(const SocketMgrData&) = delete;
    SocketMgrData& operator=(const SocketMgrData&) = delete;

    bool isRunning() const { return server_running_; }
    void setRunning(bool running) { server_running_ = running; }

    void addThread(std::shared_ptr<rpi::threads::IBaseThread>);
    void removeCompletedThreads();
    void stopAllThreads();

    std::mutex mgr_mutex;
private:
    SocketMgrData() = default;

private:
    bool server_running_ = false;
    std::vector<std::shared_ptr<rpi::threads::IBaseThread>> threads_;
};

}
}