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
    const auto callback = [&done]()
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
    const auto callback = [&done]()
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
    const auto callback = [&done]()
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

    timer_manager.set_timer(nullptr, std::chrono::system_clock::now());
    timer_manager.set_timer({}, std::chrono::system_clock::now());
    timer_manager.set_timer(std::function<void()>(nullptr), std::chrono::system_clock::now());

    // Making sure all timers are executed
    std::this_thread::sleep_for(1ms);
}

TEST(TimerManagerTest, InvalidNestedCallback)
{
    concurrent::timer_manager timer_manager;

    const auto callback = [&timer_manager]()
    {
        timer_manager.set_timer(nullptr, std::chrono::system_clock::now());
        timer_manager.set_timer({}, std::chrono::system_clock::now());
        timer_manager.set_timer(std::function<void()>(nullptr), std::chrono::system_clock::now());
    };

    timer_manager.set_timer(std::move(callback), std::chrono::system_clock::now());

    std::this_thread::sleep_for(1ms);
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
    const auto first_callback = [&first_done, &actual_queue]
    {
        actual_queue.push_back(1);
        first_done = true;
        first_done.notify_one();
    };
    // Waiting times are increasing
    constexpr auto first_wait_time = 100ms;

    std::atomic<bool> second_done{ false };
    const auto second_callback = [&second_done, &actual_queue]
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
    const auto first_callback = [&first_done, &actual_queue]
    {
        actual_queue.push_back(1);
        first_done = true;
        first_done.notify_one();
    };
    // Waiting times are decreasing
    constexpr auto first_wait_time = 200ms;

    std::atomic<bool> second_done{ false };
    const auto second_callback = [&second_done, &actual_queue]
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

    // TODO This check failed once. Why?
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
    const auto first_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(1);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };
    const auto second_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(2);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };
    const auto third_callback = [&done, &actual_queue]
    {
        actual_queue.push_back(3);
        if (actual_queue.size() == final_size)
        {
            done = true;
            done.notify_one();
        }
    };

    const auto start_time = std::chrono::system_clock::now();
    timer_manager.set_timer(std::move(first_callback), start_time);
    timer_manager.set_timer(std::move(second_callback), start_time);
    timer_manager.set_timer(std::move(third_callback), start_time);

    done.wait(false);
    const auto exec_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    EXPECT_GE(exec_time.count(), 0);
    EXPECT_LE(exec_time.count(), tolerance_time.count());

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
    const auto tricky_callback = [&]
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

TEST(TimerManagerTest, StressTest)
{
    concurrent::timer_manager timer_manager;

    constexpr size_t iteration_num = 1e4;
    constexpr auto max_wait_time = 1000ms;

    constexpr unsigned int seed = 31;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distrib(0, static_cast<int>(max_wait_time.count()));

    std::atomic<int> counter;
    const auto inc_counter_callback = [&counter] { ++counter; };

    const auto start_time = std::chrono::system_clock::now();
    for (size_t i = 0; i < iteration_num; ++i)
    {
        const int random_value = distrib(generator);
        const auto fire_time = start_time + std::chrono::milliseconds(random_value);
        timer_manager.set_timer(inc_counter_callback, fire_time);
    }

    std::this_thread::sleep_for(max_wait_time);

    EXPECT_EQ(counter.load(), iteration_num);
}
