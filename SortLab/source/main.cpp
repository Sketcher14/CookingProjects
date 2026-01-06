#include "algo/sort.h"

#include <print>
#include <random>

int main()
{
    std::vector<int> nums = { 5, 4, 3, 2, 1 };

    // static constexpr size_t size = 100;
    // nums.resize(size);
    //
    // static constexpr unsigned int seed = 31;
    // std::mt19937 generator(seed);
    // std::uniform_int_distribution distrib(0, 100);
    //
    // for (size_t i = 0; i < size; ++i)
    //     nums[i] = distrib(generator);

    algo::concurrent::merge_sort_advanced(nums);

    std::println("{}", nums);
}
