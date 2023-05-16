#ifndef RPI_SERVER_ITHREAD_H
#define RPI_SERVER_ITHREAD_H

#include <thread>
#include <pthread.h>

namespace rpi
{
namespace threads
{

class IBaseThread
{
public:
    virtual ~IBaseThread() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;
    virtual void cancel() = 0;
};

template<class Derived>
class IThread : public IBaseThread
{
public:
    void start() override
    {
        isRunning_ = true;
        thread_ = std::thread(&Derived::run, &derived());
    }

    void stop() override
    {
        isRunning_ = false;
    }

    virtual void cancel()
    {
        stop();
        pthread_cancel(thread_.native_handle());
    }

    bool isRunning() const override { return isRunning_; }

    virtual ~IThread()
    {
        if(isRunning_)
        {
            stop();
        }
        thread_.join();
    }

    Derived& derived() { return static_cast<Derived&>(*this); }

protected:
    virtual void run () = 0;

private:
    bool isRunning_ = false;
    std::thread thread_;
};

}
}

#endif