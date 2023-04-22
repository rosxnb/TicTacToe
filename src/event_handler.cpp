#include "event_handler.hpp"

void EventQueue::push(const Event& event)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push(event);
    lock.unlock();
    _condition_variable.notify_one();
}

Event& EventQueue::pop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (_queue.empty())
        _condition_variable.wait(lock);

    auto& event = _queue.front();
    _queue.pop();
    return event;
}

void event_loop(EventQueue& event_queue)
{
    while (true)
    {
        auto& event = event_queue.pop();
        event.callback();
    }
}
