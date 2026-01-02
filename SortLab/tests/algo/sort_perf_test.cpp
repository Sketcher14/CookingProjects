#include <benchmark/benchmark.h>
#include <random>

#include "algo/sort.h"

/**
 * Test Data Generators:
 *     1. Random data - typical case, measures average performance
 *     2. Already sorted data - best case for insertion sort, worst for naive quick sort
 *     3. Reverse sorted data - worst case for many algorithms
 *     4. Mostly sorted data (95% sorted)
 *     5. Many duplicates in data
 */

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


/** Benchmark template for measuring sort algorithms */
template<typename SortFunction, typename DataGenerator>
static void BM_Sort(benchmark::State& state, SortFunction sort_function, DataGenerator data_generator)
{
    const auto size = static_cast<size_t>(state.range(0));

    for (auto _: state)
    {
        // Do not measure data generation
        state.PauseTiming();
        auto data = data_generator(size);
        state.ResumeTiming();

        sort_function(data);

        benchmark::DoNotOptimize(data);
    }

    state.SetComplexityN(state.range(0));
}


/** A macros for measuring fast sort algorithms O(nlogn) */
#define BENCHMARK_SORT_FAST(sort_name, data_generator_name, sort_function, data_generator) \
    BENCHMARK_CAPTURE(BM_Sort, sort_name##_##data_generator_name##_Data, sort_function, data_generator) \
        ->RangeMultiplier(10) \
        ->Range(1e2, 1e6) \
        ->Complexity(benchmark::oNLogN) \
        ->Unit(benchmark::kMillisecond);

/** A macros for measuring slow sort algorithms O(n^2) */
#define BENCHMARK_SORT_SLOW(sort_name, data_generator_name, sort_function, data_generator) \
    BENCHMARK_CAPTURE(BM_Sort, sort_name##_##data_generator_name##_Data, sort_function, data_generator) \
        ->RangeMultiplier(10) \
        ->Range(1e2, 1e5) \
        ->Complexity(benchmark::oNSquared) \
        ->Unit(benchmark::kMillisecond);


/** Performance benchmarks for STL Sort */
BENCHMARK_SORT_FAST(STL_Sort, Random, algo::stl_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_FAST(STL_Sort, Sorted, algo::stl_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_FAST(STL_Sort, Reversed, algo::stl_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_FAST(STL_Sort, AlmostSorted, algo::stl_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_FAST(STL_Sort, Duplicated, algo::stl_sort<std::vector<int>&>, generate_duplicated_data);


/** Performance benchmarks for Quick Sort */
BENCHMARK_SORT_FAST(Quick_Sort, Random, algo::quick_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_FAST(Quick_Sort, Sorted, algo::quick_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_FAST(Quick_Sort, Reversed, algo::quick_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_FAST(Quick_Sort, AlmostSorted, algo::quick_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_FAST(Quick_Sort, Duplicated, algo::quick_sort<std::vector<int>&>, generate_duplicated_data);


/** Performance benchmarks for Merge Sort */
BENCHMARK_SORT_FAST(Merge_Sort, Random, algo::merge_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_FAST(Merge_Sort, Sorted, algo::merge_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_FAST(Merge_Sort, Reversed, algo::merge_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_FAST(Merge_Sort, AlmostSorted, algo::merge_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_FAST(Merge_Sort, Duplicated, algo::merge_sort<std::vector<int>&>, generate_duplicated_data);


/** Performance benchmarks for Bubble Sort */
BENCHMARK_SORT_SLOW(Bubble_Sort, Random, algo::bubble_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_SLOW(Bubble_Sort, Sorted, algo::bubble_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_SLOW(Bubble_Sort, Reversed, algo::bubble_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_SLOW(Bubble_Sort, AlmostSorted, algo::bubble_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(Bubble_Sort, Duplicated, algo::bubble_sort<std::vector<int>&>, generate_duplicated_data);


/** Performance benchmarks for Insertion Sort */
BENCHMARK_SORT_SLOW(Insertion_Sort, Random, algo::insertion_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_SLOW(Insertion_Sort, Sorted, algo::insertion_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_SLOW(Insertion_Sort, Reversed, algo::insertion_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_SLOW(Insertion_Sort, AlmostSorted, algo::insertion_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(Insertion_Sort, Duplicated, algo::insertion_sort<std::vector<int>&>, generate_duplicated_data);


/** Performance benchmarks for Selection Sort */
BENCHMARK_SORT_SLOW(Selection_Sort, Random, algo::selection_sort<std::vector<int>&>, generate_random_data);
BENCHMARK_SORT_SLOW(Selection_Sort, Sorted, algo::selection_sort<std::vector<int>&>, generate_sorted_data);
BENCHMARK_SORT_SLOW(Selection_Sort, Reversed, algo::selection_sort<std::vector<int>&>, generate_reversed_data);
BENCHMARK_SORT_SLOW(Selection_Sort, AlmostSorted, algo::selection_sort<std::vector<int>&>, generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(Selection_Sort, Duplicated, algo::selection_sort<std::vector<int>&>, generate_duplicated_data);


BENCHMARK_MAIN();
