#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>

struct Event 
{
    std::function< void() > callback;
};

class EventQueue
{
    public:
        void push(const Event& event);
        Event& pop();

    private:
        std::mutex _mutex;
        std::condition_variable _condition_variable;
        std::queue<Event> _queue;
};


void event_loop(EventQueue& event_queue);

#endif
