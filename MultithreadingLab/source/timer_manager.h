#pragma once

#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace concurrent
{
class timer_manager final
{
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

public:
    timer_manager();
    explicit timer_manager(std::function<void(std::exception_ptr)> exception_handler);
    ~timer_manager();

    timer_manager(const timer_manager& other) = delete;
    timer_manager& operator=(const timer_manager& other) = delete;

    timer_manager(timer_manager&& other) = delete;
    timer_manager& operator=(timer_manager&& other) = delete;

    void set_timer(std::function<void()> callback, const time_point fire_time);

private:
    struct callback_data
    {
        uint64_t id;
        std::function<void()> callback;
        time_point fire_time;

        explicit callback_data(const uint64_t in_id, std::function<void()> in_callback, const time_point in_fire_time) :
            id(in_id), callback(std::move(in_callback)), fire_time(in_fire_time)
        {}

        bool operator<(const callback_data& other) const
        {
            if (fire_time == other.fire_time)
                return id > other.id;

            return fire_time > other.fire_time;
        }
    };

    std::jthread _thread;
    std::recursive_mutex _mutex;
    std::condition_variable_any _cv;

    std::priority_queue<callback_data> _callbacks;
    uint64_t _callbacks_counter = 0;

    std::function<void(std::exception_ptr)> _exception_handler;

    void run(const std::stop_token stop_token);
};
} // namespace concurrent
