#include "timer_manager.h"

#include "gtest/gtest.h"

#include <random>

using namespace std::chrono_literals;

// All test cases use a wait time for callbacks 100 milliseconds, which is why this tolerance time has been chosen
static constexpr auto tolerance_time = 5ms;

static_assert(!std::is_copy_constructible_v<concurrent::timer_manager>);
static_assert(!std::is_copy_assignable_v<concurrent::timer_manager>);
static_assert(!std::is_move_constructible_v<concurrent::timer_manager>);
static_assert(!std::is_move_assignable_v<concurrent::timer_manager>);


TEST(TimerManagerTest, CorrectLifeSpan)
{
    concurrent::timer_manager timer_manager;
}

TEST(TimerManagerTest, CallbackFireTimeInFuture)
{
    concurrent::timer_manager timer_manager;

    std::atomic<bool> done{ false };
    auto callback = [&done]()
    {
        done = true;
        done.notify_one();
    };
    const auto start_time = std::chrono::system_clock::now();
    constexpr auto wait_time = 100ms;
    const auto callback_fire_time = start_time + wait_time;

    timer_manager.set_timer(std::move(callback), callback_fire_time);

    done.wait(false);
    const auto finish_time = std::chrono::system_clock::now();
    const auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);

    // exec_time might be slightly greater
    EXPECT_GE(exec_time.count(), wait_time.count());
    EXPECT_LE(exec_time.count(), (wait_time + tolerance_time).count());
}

TEST(TimerManagerTest, CallbackFireTimeInPresent)
{
    concurrent::timer_manager timer_manager;

    std::atomic<bool> done{ false };
    auto callback = [&done]()
    {
        done = true;
        done.notify_one();
    };
    const auto start_time = std::chrono::system_clock::now();
    const auto callback_fire_time = start_time;

    timer_manager.set_timer(std::move(callback), callback_fire_time);

    done.wait(false);
    const auto finish_time = std::chrono::system_clock::now();
    const auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);

    // exec_time might be slightly greater than expected value
    EXPECT_GE(exec_time.count(), 0);
    EXPECT_LE(exec_time.count(), tolerance_time.count());
}

TEST(TimerManagerTest, CallbackFireTimeInPast)
{
    concurrent::timer_manager timer_manager;

    std::atomic<bool> done{ false };
    auto callback = [&done]()
    {
        done = true;
        done.notify_one();
    };
    const auto start_time = std::chrono::system_clock::now();
    const auto callback_fire_time = start_time - 100ms;

    timer_manager.set_timer(std::move(callback), callback_fire_time);

    done.wait(false);
    const auto finish_time = std::chrono::system_clock::now();
    const auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);

    // exec_time might be slightly greater than expected value
    EXPECT_GE(exec_time.count(), 0);
    EXPECT_LE(exec_time.count(), tolerance_time.count());
}

TEST(TimerManagerTest, InvalidCallback)
{
    concurrent::timer_manager timer_manager;

    EXPECT_THROW(timer_manager.set_timer(nullptr, std::chrono::system_clock::now()), std::invalid_argument);
    EXPECT_THROW(timer_manager.set_timer({}, std::chrono::system_clock::now()), std::invalid_argument);
    EXPECT_THROW(timer_manager.set_timer(std::function<void()>(nullptr), std::chrono::system_clock::now()),
                 std::invalid_argument);
}

TEST(TimerManagerTest, InvalidNestedCallback)
{
    std::atomic<bool> exception_handled{ false };
    auto exception_handler = [&exception_handled](std::exception_ptr exception)
    {
        if (exception)
        {
            EXPECT_THROW(std::rethrow_exception(exception), std::invalid_argument);
            exception_handled = true;
            exception_handled.notify_one();
        }
    };
    concurrent::timer_manager timer_manager(std::move(exception_handler));

    auto callback = [&timer_manager]() { timer_manager.set_timer(nullptr, std::chrono::system_clock::now()); };

    timer_manager.set_timer(std::move(callback), std::chrono::system_clock::now());

    exception_handled.wait(false);
    EXPECT_TRUE(exception_handled.load());
}

TEST(TimerManagerTest, PassCallbackByLvalue)
{
    concurrent::timer_manager timer_manager;

    std::atomic<int> counter{ 0 };
    std::atomic<bool> done{ false };
    const auto callback = [&counter, &done]()
    {
        ++counter;
        done = true;
        done.notify_one();
    };
    timer_manager.set_timer(callback, std::chrono::system_clock::now());

    done.wait(false);

    // Calling the callback one more time because it was passed as lvalue
    callback();

    EXPECT_EQ(counter.load(), 2);
}

TEST(TimerManagerTest, CorrectExecutionOrderIncreasingTime)
{
    concurrent::timer_manager timer_manager;

    // There is no need for sync primitive here
    // because the container is changed only from the timer_manager's thread
    constexpr size_t final_size = 2;
    std::vector<int> actual_queue;
    actual_queue.reserve(final_size);

    std::atomic<bool> first_done{ false };
    auto first_callback = [&first_done, &actual_queue]
    {
        actual_queue.push_back(1);
        first_done = true;
        first_done.notify_one();
    };
    // Waiting times are increasing
    constexpr auto first_wait_time = 100ms;

    std::atomic<bool> second_done{ false };
    auto second_callback = [&second_done, &actual_queue]
    {
        actual_queue.push_back(2);
        second_done = true;
        second_done.notify_one();
    };
    constexpr auto second_wait_time = 200ms;

    const auto start_time = std::chrono::system_clock::now();
    timer_manager.set_timer(std::move(first_callback), start_time + first_wait_time);
    timer_manager.set_timer(std::move(second_callback), start_time + second_wait_time);

    // Waiting for the first callback
    first_done.wait(false);
    const auto first_exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(first_exec_time.count(), first_wait_time.count());
    EXPECT_LE(first_exec_time.count(), (first_wait_time + tolerance_time).count());

    // Waiting for the second callback
    second_done.wait(false);
    const auto second_exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(second_exec_time.count(), second_wait_time.count());
    EXPECT_LE(second_exec_time.count(), (second_wait_time + tolerance_time).count());

    std::vector<int> expected_queue{ 1, 2 };
    EXPECT_EQ(actual_queue, expected_queue);
}

TEST(TimerManagerTest, CorrectExecutionOrderDecreasingTime)
{
    concurrent::timer_manager timer_manager;

    // There is no need for sync primitive here
    // because the container is changed only from the timer_manager's thread
    constexpr size_t final_size = 2;
    std::vector<int> actual_queue;
    actual_queue.reserve(final_size);

    std::atomic<bool> first_done{ false };
    auto first_callback = [&first_done, &actual_queue]
    {
        actual_queue.push_back(1);
        first_done = true;
        first_done.notify_one();
    };
    // Waiting times are decreasing
    constexpr auto first_wait_time = 200ms;

    std::atomic<bool> second_done{ false };
    auto second_callback = [&second_done, &actual_queue]
    {
        actual_queue.push_back(2);
        second_done = true;
        second_done.notify_one();
    };
    constexpr auto second_wait_time = 100ms;

    const auto start_time = std::chrono::system_clock::now();
    timer_manager.set_timer(std::move(first_callback), start_time + first_wait_time);
    timer_manager.set_timer(std::move(second_callback), start_time + second_wait_time);

    // First, waiting for the second callback
    second_done.wait(false);
    const auto second_exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(second_exec_time.count(), second_wait_time.count());
    EXPECT_LE(second_exec_time.count(), (second_wait_time + tolerance_time).count());

    // Waiting for the first callback
    first_done.wait(false);
    const auto first_exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(first_exec_time.count(), first_wait_time.count());
    EXPECT_LE(first_exec_time.count(), (first_wait_time + tolerance_time).count());

    std::vector<int> expected_queue{ 2, 1 };
    EXPECT_EQ(actual_queue, expected_queue);
}

TEST(TimerManagerTest, CorrectExecutionOrderSameTime)
{
    concurrent::timer_manager timer_manager;

    // There is no need for sync primitive here
    // because the container is changed only from the timer_manager's thread
    constexpr size_t final_size = 3;
    std::vector<int> actual_queue;
    actual_queue.reserve(final_size);

    std::atomic<bool> done{ false };
    auto first_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(1);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };
    auto second_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(2);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };
    auto third_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(3);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };

    const auto start_time = std::chrono::system_clock::now();
    constexpr auto callbacks_wait_time = 100ms;
    const auto callbacks_fire_time = start_time + callbacks_wait_time;

    timer_manager.set_timer(std::move(first_callback), callbacks_fire_time);
    timer_manager.set_timer(std::move(second_callback), callbacks_fire_time);
    timer_manager.set_timer(std::move(third_callback), callbacks_fire_time);

    done.wait(false);
    const auto exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(exec_time.count(), callbacks_wait_time.count());
    EXPECT_LE(exec_time.count(), (callbacks_wait_time + tolerance_time).count());

    std::vector<int> expected_queue{ 1, 2, 3 };
    EXPECT_EQ(actual_queue, expected_queue);
}

TEST(TimerManagerTest, CorrectExecutionNestedSetTimer)
{
    concurrent::timer_manager timer_manager;

    // There is no need for sync primitive here
    // because the container is changed only from the timer_manager's thread
    constexpr size_t final_size = 2;
    std::vector<int> actual_queue;
    actual_queue.reserve(final_size);

    std::atomic<bool> done{ false };
    auto tricky_callback = [&]
    {
        actual_queue.push_back(1);
        timer_manager.set_timer(
                [&done, &actual_queue]
                {
                    actual_queue.push_back(2);
                    done = true;
                    done.notify_one();
                },
                std::chrono::system_clock::now());
    };

    const auto start_time = std::chrono::system_clock::now();
    timer_manager.set_timer(std::move(tricky_callback), start_time);

    done.wait(false);
    const auto exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(exec_time.count(), 0);
    EXPECT_LE(exec_time.count(), tolerance_time.count());

    std::vector<int> expected_queue{ 1, 2 };
    EXPECT_EQ(actual_queue, expected_queue);
}

TEST(TimerManagerTest, DestructionWithWaitingCallback)
{
    std::atomic<int> counter{ 0 };
    constexpr auto extra_wait_time = 100ms;
    {
        concurrent::timer_manager timer_manager;

        const auto inc_counter_callback = [&counter]
        {
            ++counter;
            counter.notify_one();
        };

        const auto start_time = std::chrono::system_clock::now();
        timer_manager.set_timer(inc_counter_callback, start_time);

        for (size_t i = 0; i < 10; ++i)
            timer_manager.set_timer(inc_counter_callback, start_time + extra_wait_time);

        // Waiting for at least first callback is executed
        counter.wait(0);
    }
    // Waiting extra time to make sure that extra callbacks were not executed
    std::this_thread::sleep_for(extra_wait_time);

    EXPECT_EQ(counter.load(), 1);
}

TEST(TimerManagerTest, MultithreadedSetTimers)
{
    concurrent::timer_manager timer_manager;

    constexpr size_t threads_num = 10;
    constexpr size_t iterations_per_thread = 1000;
    constexpr size_t total_iterations = threads_num * iterations_per_thread;

    constexpr int max_distribution = 100;
    constexpr auto max_callback_wait_time = std::chrono::milliseconds(max_distribution);

    std::vector<std::jthread> threads;
    threads.reserve(threads_num);

    std::atomic<int> counter{ 0 };
    const auto inc_counter_callback = [&counter] { ++counter; };

    for (size_t i = 0; i < threads_num; ++i)
    {
        constexpr unsigned int seed = 31;
        threads.emplace_back(
                [&, seed = seed + static_cast<unsigned int>(i)]
                {
                    std::mt19937 generator(seed);
                    std::uniform_int_distribution<int> distrib(0, max_distribution);
                    const auto start_time = std::chrono::system_clock::now();

                    for (size_t j = 0; j < iterations_per_thread; ++j)
                    {
                        const auto wait_time = std::chrono::milliseconds(distrib(generator));
                        timer_manager.set_timer(inc_counter_callback, start_time + wait_time);
                    }
                });
    }

    for (auto& thread: threads)
        thread.join();

    // Waiting for all callbacks to fire
    std::this_thread::sleep_for(max_callback_wait_time + tolerance_time);

    EXPECT_EQ(counter.load(), total_iterations);
}

TEST(TimerManagerTest, CallbackThrowsException)
{
    std::atomic<bool> exception_handled{ false };
    auto exception_handler = [&exception_handled](std::exception_ptr exception)
    {
        if (exception)
        {
            exception_handled = true;
            exception_handled.notify_one();
        }
    };
    concurrent::timer_manager timer_manager(std::move(exception_handler));

    std::atomic<bool> done{ false };
    auto good_callback = [&done]
    {
        done = true;
        done.notify_one();
    };
    auto bad_callback = [] { throw std::runtime_error("Error"); };

    const auto start_time = std::chrono::system_clock::now();
    timer_manager.set_timer(std::move(bad_callback), start_time);
    timer_manager.set_timer(std::move(good_callback), start_time + 100ms);

    // Waiting when an exception from the bad_callback will be handled
    exception_handled.wait(false);
    EXPECT_TRUE(exception_handled.load());

    // Waiting when the good_callback will be executed. The bad_callback doesn't interrupt it.
    done.wait(false);
    EXPECT_TRUE(done.load());
}
