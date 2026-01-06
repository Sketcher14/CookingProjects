#include "utility.h"

#include <random>

namespace utility::tests
{
    // There is no need in random seed because it may lead to different performance results
    static constexpr unsigned int seed = 31;
    std::mt19937 generator(seed);

    std::vector<int> generate_random_data(const size_t size)
    {
        std::uniform_int_distribution distrib; // [0, INT_MAX]

        std::vector<int> data(size);

        for (size_t i = 0; i < size; ++i)
            data[i] = distrib(generator);

        return data;
    }

    std::vector<int> generate_sorted_data(const size_t size)
    {
        std::vector<int> data(size);

        for (size_t i = 0; i < size; ++i)
            data[i] = static_cast<int>(i);

        return data;
    }

    std::vector<int> generate_reversed_data(const size_t size)
    {
        std::vector<int> data(size);

        for (size_t i = 0; i < size; ++i)
            data[i] = static_cast<int>(size - i - 1);

        return data;
    }

    std::vector<int> generate_almost_sorted_data(const size_t size)
    {
        std::vector<int> data(size);

        for (size_t i = 0; i < size; ++i)
            data[i] = static_cast<int>(i);

        std::uniform_int_distribution<size_t> distrib(0, size - 1);

        // Swapping 5% of elements
        const size_t swap_num = size / 20;
        for (size_t i = 0; i < swap_num; ++i)
            std::swap(data[distrib(generator)], data[distrib(generator)]);

        return data;
    }

    std::vector<int> generate_duplicated_data(const size_t size)
    {
        std::uniform_int_distribution distrib(0, 50); // only 50 unique values

        std::vector<int> data(size);

        for (size_t i = 0; i < size; ++i)
            data[i] = distrib(generator);

        return data;
    }
}