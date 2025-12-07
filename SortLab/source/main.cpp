#include <iostream>

#include "algo/sort.h"

int main()
{
    std::vector<int> nums {5, 4, 3, 2, 1};

    algo::stl_sort(nums);

    for (const int num : nums)
        std::cout << num << ' ';

    return 0;
}
