#include <algorithm>
#include <iostream>

#include "socketManager/SocketMgrData.h"

namespace rpi
{
namespace socket_manager
{

SocketMgrData& SocketMgrData::getInstance()
{
    static SocketMgrData inst;
    return inst;
}

void SocketMgrData::addThread(std::shared_ptr<rpi::threads::IBaseThread> pThread)
{
    threads_.push_back(pThread);
}

void SocketMgrData::removeCompletedThreads()
{
    threads_.erase(
        std::remove_if(threads_.begin(), threads_.end(), 
            [](const std::shared_ptr<rpi::threads::IBaseThread> &obj)
            {
                return (obj->isRunning() == false);
            }
        ),
        threads_.end()
    );
}

void SocketMgrData::stopAllThreads()
{
    for(auto &it : threads_)
    {
        it->cancel();
    }
    threads_.clear();
}

}
}
