#include <iostream>

#include "algo/sort.h"
#include "print.h"

int main()
{
    std::vector<int> nums {5, 5, 5};
    std::vector<float> nums1 {5.1, 4.2, 3.3, 2.4, 1.5};

    algo::quick_sort(nums1);

    for (const int num : nums)
        print("{} ", num);
    print("\n");


    return 0;
}
