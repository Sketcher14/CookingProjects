#include "thread_pool.h"

#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <vector>

static_assert(!std::is_copy_constructible_v<concurrent::thread_pool>);
static_assert(!std::is_copy_assignable_v<concurrent::thread_pool>);
static_assert(!std::is_move_constructible_v<concurrent::thread_pool>);
static_assert(!std::is_move_assignable_v<concurrent::thread_pool>);

TEST(ThreadPoolTest, ZeroSizePool)
{
    EXPECT_THROW(concurrent::thread_pool(0), std::invalid_argument);
}

TEST(ThreadPoolTest, NotEmptyPool)
{
    concurrent::thread_pool pool(10);
}

TEST(ThreadPoolTest, PushLambda)
{
    concurrent::thread_pool pool(1);
    auto future = pool.push_task([](const int a, const int b) { return a * b; }, 3, 4);
    EXPECT_EQ(future.get(), 12);
}

TEST(ThreadPoolTest, PushCapturingLambda)
{
    concurrent::thread_pool pool(1);
    int multiplier = 3;
    auto future = pool.push_task([multiplier](const int x) { return x * multiplier; }, 4);
    EXPECT_EQ(future.get(), 12);
}

TEST(ThreadPoolTest, PushVoidLambda)
{
    concurrent::thread_pool pool(1);
    std::atomic<bool> executed{ false };
    auto future = pool.push_task([&executed]() { executed = true; });
    future.get();
    EXPECT_TRUE(executed);
}

int global_multiply(const int a, const int b)
{
    return a * b;
}

TEST(ThreadPoolTest, PushGlobalFunction)
{
    concurrent::thread_pool pool(1);
    auto future = pool.push_task(global_multiply, 3, 4);
    EXPECT_EQ(future.get(), 12);
}

struct callable_object
{
    int operator()(const int a, const int b) const
    {
        return a * b;
    }
};

TEST(ThreadPoolTest, PushCallableObject)
{
    concurrent::thread_pool pool(1);
    callable_object callable;
    auto future = pool.push_task(callable, 3, 4);
    EXPECT_EQ(future.get(), 12);
}

class test_class
{
public:
    int multiply(const int a, const int b) const
    {
        return a * b;
    }
};

TEST(ThreadPoolTest, PushMemberFunction)
{
    concurrent::thread_pool pool(1);
    test_class object;
    auto future = pool.push_task(std::bind_front(&test_class::multiply, object), 3, 4);
    EXPECT_EQ(future.get(), 12);
}

TEST(ThreadPoolTest, PushFunctionAsLvalue)
{
    concurrent::thread_pool pool(1);
    auto copyable_foo = [](const int a, const int b) { return a * b; };
    auto future = pool.push_task(copyable_foo, 3, 4);
    EXPECT_EQ(future.get(), 12);
    EXPECT_EQ(copyable_foo(3, 4), 12);
}

TEST(ThreadPoolTest, PushFunctionAsRvalue)
{
    concurrent::thread_pool pool(1);
    auto movable_foo = [](const int a, const int b) { return a * b; };
    auto future = pool.push_task(std::move(movable_foo), 3, 4);
    EXPECT_EQ(future.get(), 12);
}

TEST(ThreadPoolTest, TaskPushesAnotherTask)
{
    concurrent::thread_pool pool(2);
    auto future = pool.push_task(
            [&pool](const int multiplier)
            {
                auto inner_future = pool.push_task([](const int a, const int b) { return a * b; }, 3, 4);
                return inner_future.get() * multiplier;
            },
            2);
    EXPECT_EQ(future.get(), 24);
}

TEST(ThreadPoolTest, MoreTasksThanThreads)
{
    constexpr size_t task_num = 50;
    concurrent::thread_pool pool(5);
    std::atomic<size_t> counter{ 0 };

    std::vector<std::future<void>> futures;
    futures.reserve(task_num);

    for (size_t i = 0; i < task_num; ++i)
        futures.push_back(pool.push_task([&counter]() { ++counter; }));

    for (auto& future: futures)
        future.get();

    EXPECT_EQ(counter.load(), task_num);
}

TEST(ThreadPoolTest, TasksRunInParallel)
{
    concurrent::thread_pool pool(4);
    std::atomic<int> concurrent_count{ 0 };
    std::atomic<int> max_concurrent{ 0 };

    constexpr size_t task_num = 4;
    std::vector<std::future<void>> futures;
    futures.reserve(task_num);

    for (size_t i = 0; i < task_num; ++i)
    {
        futures.push_back(pool.push_task(
                [&]()
                {
                    int current = ++concurrent_count;

                    // Update max if we have more concurrent tasks than before
                    int prev_max = max_concurrent.load();
                    while (prev_max < current && !max_concurrent.compare_exchange_weak(prev_max, current))
                    {
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    --concurrent_count;
                }));
    }

    for (auto& future: futures)
        future.get();

    // If max_active > 1, multiple tasks ran in parallel
    EXPECT_GT(max_concurrent.load(), 1);
}

TEST(ThreadPoolTest, StressTest)
{
    unsigned int thread_num = std::thread::hardware_concurrency();
    if (thread_num == 0)
        thread_num = 4;
    concurrent::thread_pool pool(thread_num);

    constexpr size_t task_num = 10000;
    std::atomic<size_t> counter{ 0 };
    std::vector<std::future<void>> futures;
    futures.reserve(task_num);

    for (size_t i = 0; i < task_num; ++i)
        futures.push_back(pool.push_task([&counter]() { ++counter; }));

    for (auto& future: futures)
        future.get();

    EXPECT_EQ(counter.load(), task_num);
}

TEST(ThreadPoolTest, ExceptionDoesNotAffectOtherTasks)
{
    concurrent::thread_pool pool(2);
    auto bad_future = pool.push_task([]() { throw std::runtime_error("Error"); });
    auto good_future = pool.push_task([]() { return 10; });

    EXPECT_THROW(bad_future.get(), std::runtime_error);
    EXPECT_EQ(good_future.get(), 10);
}

TEST(ThreadPoolTest, DestructionWithPendingTasks)
{
    std::atomic<int> started{ 0 };
    std::atomic<int> completed{ 0 };
    {
        concurrent::thread_pool pool(1);
        std::atomic<bool> firs_task_started{ false };
        // Pushing the first long task
        pool.push_task(
                [&]()
                {
                    ++started;
                    firs_task_started = true;
                    firs_task_started.notify_one();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    ++completed;
                });
        // Pushing more tasks while first is running
        for (size_t i = 0; i < 10; ++i)
        {
            pool.push_task(
                    [&]()
                    {
                        ++started;
                        ++completed;
                    });
        }
        firs_task_started.wait(false);
    }
    EXPECT_EQ(started.load(), 1);
    EXPECT_EQ(started.load(), completed.load());
}
