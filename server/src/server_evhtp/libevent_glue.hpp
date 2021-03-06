#pragma once

#include "../defines.hpp"
#include "../system.hpp"
#include "../chrono.hpp"
#include "../work_queue.hpp"
#include "../timers.hpp"

#include <utility>
#include <functional>
#include <memory>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/thread.h>

namespace msrv {
namespace server_evhtp {

class Event;
class EventBase;

using EventPtr = std::unique_ptr<Event>;
using EventCallback = std::function<void(Event*, int)>;

class Event
{
public:
    Event(
        EventBase* base,
        SocketHandle socket = SocketHandle(),
        int events = 0,
        EventCallback callback = EventCallback());

    Event(EventBase* base, EventCallback callback)
        : Event(base, SocketHandle(), 0, callback) { }

    ~Event();

    EventBase* base() { return base_; }
    ::event* ptr() { return ptr_; }

    void setCallback(EventCallback callback) { callback_ = std::move(callback); }
    void schedule(DurationMs timeout = DurationMs::zero());
    void unschedule();

private:
    static void runCallback(evutil_socket_t, short, void*);

    EventBase* base_;
    ::event* ptr_;
    EventCallback callback_;

    MSRV_NO_COPY_AND_ASSIGN(Event);
};

class EventBase
{
public:
    explicit EventBase(bool threadSafe = true);
    ~EventBase();

    ::event_base* ptr() { return ptr_; }

    bool runLoop(int flags = 0);
    void exitLoop();

private:
    ::event_base* ptr_;

    MSRV_NO_COPY_AND_ASSIGN(EventBase);
};

class Evbuffer
{
public:
    Evbuffer();

    Evbuffer(Evbuffer&&) = default;

    Evbuffer(::evbuffer* ptr, bool owned = true)
        : ptr_(ptr), owned_(owned) { }

    ~Evbuffer();

    Evbuffer& operator=(Evbuffer&&) = default;

    ::evbuffer* ptr() { return ptr_; }

    size_t length() { return evbuffer_get_length(ptr()); }

    std::vector<char> readToEnd();

    void write(const char*, size_t);
    void write(const std::string& str) { write(str.data(), str.length()); }
    void writeFile(FileHandle file, int64_t offset, int64_t size);

    template<size_t N>
    void write(char data[N]) { write(data, N - 1); }

private:
    ::evbuffer* ptr_;
    bool owned_;

    MSRV_NO_COPY_AND_ASSIGN(Evbuffer);
};

class EventBaseWorkQueue final : public ExternalWorkQueue
{
public:
    explicit EventBaseWorkQueue(EventBase* base);
    ~EventBaseWorkQueue();

protected:
    virtual void schedule() override { notifyEvent_.schedule(); }

private:
    Event notifyEvent_;
};

class EventTimer final : public Timer
{
public:
    EventTimer(EventBase* eventBase);
    virtual ~EventTimer();

    virtual TimerState state() const override { return state_; }
    virtual DurationMs period() const override { return period_; }

    virtual void setCallback(TimerCallback callback) override { callback_ = std::move(callback); }
    virtual void runOnce(DurationMs delay) override;
    virtual void runPeriodic(DurationMs period) override;
    virtual void stop() override;

private:
    void run();

    Event event_;
    TimerState state_;
    DurationMs period_;
    TimerCallback callback_;
};

class EventTimerFactory final : public TimerFactory
{
public:
    EventTimerFactory(EventBase* eventBase)
        : eventBase_(eventBase)
    {
    }

    virtual ~EventTimerFactory();

    virtual TimerPtr createTimer() override
    {
        return std::make_unique<EventTimer>(eventBase_);
    }

private:
    EventBase* eventBase_;
};

}}
