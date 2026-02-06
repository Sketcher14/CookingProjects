#include "timer_manager.h"

namespace concurrent
{
timer_manager::timer_manager() : timer_manager(nullptr)
{}
timer_manager::timer_manager(std::function<void(std::exception_ptr)> exception_handler) :
    _exception_handler(std::move(exception_handler))
{
    _thread = std::jthread(std::bind_front(&timer_manager::run, this));
}

timer_manager::~timer_manager()
{
    _thread.request_stop();

    if (_thread.joinable())
        _thread.join();
}

void timer_manager::set_timer(std::function<void()> callback, const time_point fire_time)
{
    if (!callback)
        throw std::invalid_argument("Callback must be callable");

    bool notify = false;
    {
        std::scoped_lock lock(_mutex);
        notify = _callbacks.empty() || fire_time < _callbacks.top().fire_time;
        _callbacks.emplace(_callbacks_counter++, std::move(callback), fire_time);
    }

    if (notify)
        _cv.notify_one();
}

void timer_manager::run(const std::stop_token stop_token)
{
    while (true)
    {
        std::unique_lock lock(_mutex);

        const time_point top_fire_time = _callbacks.empty() ? time_point::max() : _callbacks.top().fire_time;

        _cv.wait_until(lock, stop_token, top_fire_time, [this]() { return !_callbacks.empty(); });

        if (stop_token.stop_requested())
            break;

        while (!_callbacks.empty())
        {
            const time_point next_fire_time = _callbacks.top().fire_time;

            if (std::chrono::system_clock::now() >= next_fire_time)
            {
                auto callback = _callbacks.top().callback;
                _callbacks.pop();

                try
                {
                    // Callback might have nested set_timer with invalid callback
                    if (callback)
                        callback();
                    else
                        throw std::invalid_argument("Callback must be callable");
                } catch (...)
                {
                    if (_exception_handler)
                        _exception_handler(std::current_exception());
                }
            }
            else
            {
                break;
            }
        }
    }
}
} // namespace concurrent
