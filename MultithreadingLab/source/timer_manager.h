#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>


class timer_manager final
{
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

public:
    timer_manager();
    ~timer_manager();

    void set_timer(std::function<void()> callback, const time_point fire_time);

private:
    struct callback_data
    {
        std::function<void()> callback;
        time_point fire_time;

        explicit callback_data(std::function<void()> in_callback, const time_point in_fire_time) :
            callback(std::move(in_callback)), fire_time(in_fire_time)
        {}

        bool operator<(const callback_data& other) const
        {
            return fire_time > other.fire_time;
        }
    };

    std::jthread _thread;
    std::recursive_mutex _mutex;
    std::condition_variable_any _cv;

    std::priority_queue<callback_data> _callbacks;

    void run(const std::stop_token stop_token);
};
