#include <iostream>

#include "algo/sort.h"

using namespace std;

int main()
{
    vector<int> nums {5, 4, 3, 2, 1};

    algo::stl_sort(nums);

    for (int num : nums)
        cout << num << ' ';
    return 0;
}
