#include "print.h"
#include "accumulation.h"

int main()
{
    const std::vector<int> nums { 1, 2, 3, 4, 5 };
    print("sum {}", multithreading::accumulate(nums));
}
