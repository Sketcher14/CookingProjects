#pragma once

#include <vector>

namespace test_utility
{
    std::vector<int> generate_random_data(const size_t size);
    std::vector<int> generate_sorted_data(const size_t size);
    std::vector<int> generate_reversed_data(const size_t size);
    std::vector<int> generate_almost_sorted_data(const size_t size);
    std::vector<int> generate_duplicated_data(const size_t size);
}
