#include <iostream>

#include "algo/sort.h"
#include "print.h"

int main()
{
    std::vector<int> nums {5, 4, 3, 2, 1};
    std::vector<float> nums1 {5.1, 4.2, 3.3, 2.4, 1.5};

    algo::stl_sort(nums);
    algo::stl_sort(nums1);

    for (const int num : nums)
        print("{} ", num);
    print("\n");

    for (const float num : nums1)
        print("{} ", num);
    print("\n");

    return 0;
}
