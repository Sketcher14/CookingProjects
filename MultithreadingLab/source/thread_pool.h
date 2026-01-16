#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace concurrent
{
class thread_pool final
{
public:
    explicit thread_pool(const size_t size);

    thread_pool(const thread_pool& other) = delete;
    thread_pool& operator=(const thread_pool& other) = delete;

    thread_pool(thread_pool&& other) = delete;
    thread_pool& operator=(thread_pool&& other) = delete;

    ~thread_pool();

    template<class F, class... Args>
    auto push_task(F&& function, Args&&... args) -> std::future<decltype(function(args...))>
    {
        using ReturnType = decltype(function(args...));

        // Binding function with its args to one callable object
        std::function<ReturnType()> callable = std::bind(std::forward<F>(function), std::forward<Args>(args)...);
        // Creating a packaged task to provide the future object.
        // It is encapsulated into a shared ptr in order to have a copy semantic
        auto task_ptr = std::make_shared<std::packaged_task<ReturnType()>>(std::move(callable));

        // Getting the task's future for return
        std::future<ReturnType> result_future = task_ptr->get_future();

        {
            std::scoped_lock lock(_tasks_mutex);
            // Wrapping the task into void function to store any type of functions in the queue
            _tasks.emplace([task_ptr]
            {
                (*task_ptr)();
            });
        }

        // Waking up one waiting thread
        _cv.notify_one();

        return result_future;
    }

private:
    std::queue<std::function<void()>> _tasks;
    std::mutex _tasks_mutex;
    std::condition_variable_any _cv;

    std::vector<std::jthread> _threads;

    void run(const std::stop_token stop_token);
};
} // namespace concurrent
