#include "thread_pool.h"

#include <stdexcept>

namespace concurrent
{
thread_pool::thread_pool(const size_t size)
{
    if (size == 0)
        throw std::invalid_argument("thread_pool size must be greater than 0");

    _threads.reserve(size);

    for (size_t i = 0; i < size; ++i)
        _threads.emplace_back(std::bind_front(&thread_pool::run, this));
}

thread_pool::~thread_pool()
{
    // Requesting all threads to stop
    for (auto& thread: _threads)
        thread.request_stop();

    // Waking up all waiting threads
    _cv.notify_all();

    // Explicitly join all threads before the mutex and cv are destructed.
    for (auto& thread: _threads)
    {
        if (thread.joinable())
            thread.join();
    }
}

void thread_pool::run(const std::stop_token stop_token)
{
    while (true)
    {
        std::function<void()> next_task;

        {
            std::unique_lock lock(_tasks_mutex);
            _cv.wait(lock, stop_token, [this]()
            {
                return !_tasks.empty();
            });

            // Tasks are not performed after a stop request
            if (stop_token.stop_requested())
                break;

            next_task = std::move(_tasks.front());
            _tasks.pop();
        }

        next_task();
    }
}
} // concurrent
