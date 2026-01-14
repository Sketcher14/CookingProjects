#include "accumulation.h"

#include <print>

int main()
{
    const std::vector<int> nums{ 1, 2, 3, 4, 5 };
    std::println("sum {}", multithreading::accumulate(nums));
}
