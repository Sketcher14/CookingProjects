#include <gtest/gtest.h>
#include <algorithm>

#include "algo/sort.h"
#include "utility.h"

struct AlgoSortTestCase
{
    std::vector<int> input;
    std::vector<int> expected;
    std::string description;
};

// Parameterized test fixture
class AlgoSortTest : public ::testing::TestWithParam<std::tuple<std::function<void(std::vector<int>&)>,
            AlgoSortTestCase>> {};

// Test cases covering various scenarios
std::vector<AlgoSortTestCase> GetTestCases()
{
    std::vector<AlgoSortTestCase> test_cases {
        // Test 1: Empty array
        { {}, {}, "Empty array" },

        // Test 2: Single element
        { { 42 }, { 42 }, "Single element" },

        // Test 3: Two elements - unsorted
        { { 2, 1 }, { 1, 2 }, "Two elements unsorted" },

        // Test 4: Two elements - sorted
        { { 1, 2 }, { 1, 2 }, "Two elements sorted" },

        // Test 5: Basic reverse sorted
        { { 5, 4, 3, 2, 1 }, { 1, 2, 3, 4, 5 }, "Reverse sorted array" },

        // Test 6: Already sorted
        { { 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5 }, "Already sorted array" },

        // Test 7: All duplicates
        { { 5, 5, 5, 5, 5 }, { 5, 5, 5, 5, 5 }, "All same elements" },

        // Test 8: Negative numbers
        { { -5, -1, -3, -2, -4 }, { -5, -4, -3, -2, -1 }, "All negative numbers" },

        // Test 9: With duplicates, odd number (9)
        { { -3, 1, 4, 1, 5, 9, 2, 0, -3 }, { -3, -3, 0, 1, 1, 2, 4, 5, 9 }, "Array with duplicates, odd number" },

        // Test 10: With duplicates, even number (10)
        {
            { -3, 1, 9, 1, 0, 9, -10, 0, -3, -10 }, { -10, -10, -3, -3, 0, 0, 1, 1, 9, 9 },
            "Array with duplicates, odd number"
        },
    };

    // Adding 5 more test cases with large number of elements for multithreaded sort algorithms
    static constexpr size_t size = 1e4;
    test_cases.reserve(test_cases.size() + 5);

    {
        // Test 11: Large random array
        AlgoSortTestCase large_random_test;
        large_random_test.description = "Large random array";
        auto data = utility::tests::generate_random_data(size);
        large_random_test.input = data;

        std::ranges::sort(data);
        large_random_test.expected = data;

        test_cases.push_back(std::move(large_random_test));
    }

    {
        // Test 12: Large already sorted array
        AlgoSortTestCase large_sorted_test;
        large_sorted_test.description = "Large already sorted array";
        auto data = utility::tests::generate_sorted_data(size);
        large_sorted_test.input = data;
        large_sorted_test.expected = data;

        test_cases.push_back(std::move(large_sorted_test));
    }

    {
        // Test 13: Large reverse sorted array
        AlgoSortTestCase large_reverse_sorted_test;
        large_reverse_sorted_test.description = "Large reverse sorted array";
        auto data = utility::tests::generate_reversed_data(size);
        large_reverse_sorted_test.input = data;

        std::ranges::sort(data);
        large_reverse_sorted_test.expected = data;

        test_cases.push_back(std::move(large_reverse_sorted_test));
    }

    {
        // Test 14: Large almost sorted array
        AlgoSortTestCase large_almost_sorted_test;
        large_almost_sorted_test.description = "Large almost sorted array";
        auto data = utility::tests::generate_almost_sorted_data(size);
        large_almost_sorted_test.input = data;

        std::ranges::sort(data);
        large_almost_sorted_test.expected = data;

        test_cases.push_back(std::move(large_almost_sorted_test));
    }

    {
        // Test 15: Large array with duplicates
        AlgoSortTestCase large_duplicated_test;
        large_duplicated_test.description = "Large array with duplicates";
        auto data = utility::tests::generate_duplicated_data(size);
        large_duplicated_test.input = data;

        std::ranges::sort(data);
        large_duplicated_test.expected = data;

        test_cases.push_back(std::move(large_duplicated_test));
    }

    return test_cases;
}

TEST_P(AlgoSortTest, SortsCorrectly)
{
    auto [sort_func, test_case] = GetParam();
    sort_func(test_case.input);
    EXPECT_EQ(test_case.input, test_case.expected) << "Failed for: " << test_case.description;
}

/** Instantiate tests for Bubble sort */
INSTANTIATE_TEST_SUITE_P(
    BubbleSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::bubble_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Insertion sort */
INSTANTIATE_TEST_SUITE_P(
    InsertionSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::insertion_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Selection sort */
INSTANTIATE_TEST_SUITE_P(
    SelectionSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::selection_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for STL sort */
INSTANTIATE_TEST_SUITE_P(
    STLSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::stl_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Boost Spread sort */
INSTANTIATE_TEST_SUITE_P(
    BoostSpreadSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::boost_spread_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Boost Spin sort */
INSTANTIATE_TEST_SUITE_P(
    BoostSpinSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::boost_spin_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Quick sort */
INSTANTIATE_TEST_SUITE_P(
    QuickSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::quick_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Merge sort */
INSTANTIATE_TEST_SUITE_P(
    MergeSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::merge_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

/** Instantiate tests for Multithreaded Merge sort */
INSTANTIATE_TEST_SUITE_P(
    MultithreadedMergeSort,
    AlgoSortTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(
            [](std::vector<int>& vector){ algo::concurrent::merge_sort(vector); }
        )),
        ::testing::ValuesIn(GetTestCases())
    )
);

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
