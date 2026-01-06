#include <benchmark/benchmark.h>

#include "algo/sort.h"
#include "utility.h"

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


/**
 * Test Data:
 *     1. Random data - typical case, measures average performance
 *     2. Already sorted data - best case for insertion sort, worst for naive quick sort
 *     3. Reverse sorted data - worst case for many algorithms
 *     4. Mostly sorted data (95% sorted)
 *     5. Many duplicates in data
 */

/** Performance benchmarks for Bubble Sort */
BENCHMARK_SORT_SLOW(BubbleSort, Random, algo::bubble_sort<std::vector<int>&>, utility::tests::generate_random_data);
BENCHMARK_SORT_SLOW(BubbleSort, Sorted, algo::bubble_sort<std::vector<int>&>, utility::tests::generate_sorted_data);
BENCHMARK_SORT_SLOW(BubbleSort, Reversed, algo::bubble_sort<std::vector<int>&>, utility::tests::generate_reversed_data);
BENCHMARK_SORT_SLOW(BubbleSort, AlmostSorted, algo::bubble_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(BubbleSort, Duplicated, algo::bubble_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for Insertion Sort */
BENCHMARK_SORT_SLOW(InsertionSort, Random, algo::insertion_sort<std::vector<int>&>,
                    utility::tests::generate_random_data);
BENCHMARK_SORT_SLOW(InsertionSort, Sorted, algo::insertion_sort<std::vector<int>&>,
                    utility::tests::generate_sorted_data);
BENCHMARK_SORT_SLOW(InsertionSort, Reversed, algo::insertion_sort<std::vector<int>&>,
                    utility::tests::generate_reversed_data);
BENCHMARK_SORT_SLOW(InsertionSort, AlmostSorted, algo::insertion_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(InsertionSort, Duplicated, algo::insertion_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for Selection Sort */
BENCHMARK_SORT_SLOW(SelectionSort, Random, algo::selection_sort<std::vector<int>&>,
                    utility::tests::generate_random_data);
BENCHMARK_SORT_SLOW(SelectionSort, Sorted, algo::selection_sort<std::vector<int>&>,
                    utility::tests::generate_sorted_data);
BENCHMARK_SORT_SLOW(SelectionSort, Reversed, algo::selection_sort<std::vector<int>&>,
                    utility::tests::generate_reversed_data);
BENCHMARK_SORT_SLOW(SelectionSort, AlmostSorted, algo::selection_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_SLOW(SelectionSort, Duplicated, algo::selection_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for STL Sort */
BENCHMARK_SORT_FAST(STLSort, Random, algo::stl_sort<std::vector<int>&>, utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(STLSort, Sorted, algo::stl_sort<std::vector<int>&>, utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(STLSort, Reversed, algo::stl_sort<std::vector<int>&>, utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(STLSort, AlmostSorted, algo::stl_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(STLSort, Duplicated, algo::stl_sort<std::vector<int>&>, utility::tests::generate_duplicated_data);


/** Performance benchmarks for Boost Spread sort */
BENCHMARK_SORT_FAST(BoostSpreadSort, Random, algo::boost_spread_sort<std::vector<int>&>,
                    utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(BoostSpreadSort, Sorted, algo::boost_spread_sort<std::vector<int>&>,
                    utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(BoostSpreadSort, Reversed, algo::boost_spread_sort<std::vector<int>&>,
                    utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(BoostSpreadSort, AlmostSorted, algo::boost_spread_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(BoostSpreadSort, Duplicated, algo::boost_spread_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for Boost Spin sort */
BENCHMARK_SORT_FAST(BoostSpinSort, Random, algo::boost_spin_sort<std::vector<int>&>,
                    utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(BoostSpinSort, Sorted, algo::boost_spin_sort<std::vector<int>&>,
                    utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(BoostSpinSort, Reversed, algo::boost_spin_sort<std::vector<int>&>,
                    utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(BoostSpinSort, AlmostSorted, algo::boost_spin_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(BoostSpinSort, Duplicated, algo::boost_spin_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);

/** Performance benchmarks for Quick Sort */
BENCHMARK_SORT_FAST(QuickSort, Random, algo::quick_sort<std::vector<int>&>, utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(QuickSort, Sorted, algo::quick_sort<std::vector<int>&>, utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(QuickSort, Reversed, algo::quick_sort<std::vector<int>&>, utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(QuickSort, AlmostSorted, algo::quick_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(QuickSort, Duplicated, algo::quick_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for Merge Sort */
BENCHMARK_SORT_FAST(MergeSort, Random, algo::merge_sort<std::vector<int>&>, utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(MergeSort, Sorted, algo::merge_sort<std::vector<int>&>, utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(MergeSort, Reversed, algo::merge_sort<std::vector<int>&>, utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(MergeSort, AlmostSorted, algo::merge_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(MergeSort, Duplicated, algo::merge_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);


/** Performance benchmarks for Multithreaded Merge Sort */
BENCHMARK_SORT_FAST(MultithreadedMergeSort, Random, algo::concurrent::merge_sort<std::vector<int>&>,
                    utility::tests::generate_random_data);
BENCHMARK_SORT_FAST(MultithreadedMergeSort, Sorted, algo::concurrent::merge_sort<std::vector<int>&>,
                    utility::tests::generate_sorted_data);
BENCHMARK_SORT_FAST(MultithreadedMergeSort, Reversed, algo::concurrent::merge_sort<std::vector<int>&>,
                    utility::tests::generate_reversed_data);
BENCHMARK_SORT_FAST(MultithreadedMergeSort, AlmostSorted, algo::concurrent::merge_sort<std::vector<int>&>,
                    utility::tests::generate_almost_sorted_data);
BENCHMARK_SORT_FAST(MultithreadedMergeSort, Duplicated, algo::concurrent::merge_sort<std::vector<int>&>,
                    utility::tests::generate_duplicated_data);

//--benchmark_filter=<regex>
BENCHMARK_MAIN();
