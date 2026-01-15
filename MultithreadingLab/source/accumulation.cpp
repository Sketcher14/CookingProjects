#include "accumulation.h"

#include <cassert>
#include <iostream>
#include <numeric>
#include <thread>

namespace concurrent
{
int accumulate(const std::vector<int>& nums)
{
    static constexpr size_t min_size_for_threading = 1000;
    if (nums.size() < min_size_for_threading)
        return std::accumulate(nums.begin(), nums.end(), 0);

    size_t num_workers = std::thread::hardware_concurrency();
    if (num_workers == 0)
        num_workers = 2;

    const size_t nums_part_size = nums.size() / num_workers;

    std::atomic_int total_sum{ 0 };

    const auto accumulate_part = [&nums, &total_sum](const size_t start, const size_t end) -> void
    {
        assert(start < end);
        assert(end <= nums.size());

        int local_sum = 0;
        for (size_t i = start; i < end; ++i)
            local_sum += nums[i];

        total_sum.fetch_add(local_sum, std::memory_order_relaxed);
    };

    std::vector<std::thread> workers;
    workers.reserve(num_workers);

    size_t start = 0;
    while (workers.size() < num_workers && start < nums.size())
    {
        size_t end = std::min(start + nums_part_size, nums.size());
        if (workers.empty())
            end += nums.size() - num_workers * nums_part_size;

        workers.emplace_back(accumulate_part, start, end);
        start = end;
    }

    for (std::thread& worker: workers)
    {
        if (worker.joinable())
            worker.join();
    }

    return total_sum.load();
}
} // namespace concurrent
