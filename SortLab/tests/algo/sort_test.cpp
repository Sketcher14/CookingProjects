#include <gtest/gtest.h>
#include <functional>

#include "algo/sort.h"

// Test data structure
struct SortTestCase
{
    std::vector<int> input;
    std::vector<int> expected;
    std::string description;
};

// Parameterized test fixture
class SortAlgorithmTest : public ::testing::TestWithParam<std::tuple<std::function<void(std::vector<int>&)>,
            SortTestCase> > {};

// Test cases covering various scenarios
std::vector<SortTestCase> GetTestCases()
{
    return {
        // Test 1: Basic reverse sorted
        { { 5, 4, 3, 2, 1 }, { 1, 2, 3, 4, 5 }, "Reverse sorted array" },

        // Test 2: Already sorted
        { { 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5 }, "Already sorted array" },

        // Test 3: Single element
        { { 42 }, { 42 }, "Single element" },

        // Test 4: Empty array
        { {}, {}, "Empty array" },

        // Test 5: Two elements - unsorted
        { { 2, 1 }, { 1, 2 }, "Two elements unsorted" },

        // Test 6: Two elements - sorted
        { { 1, 2 }, { 1, 2 }, "Two elements sorted" },

        // Test 7: All duplicates
        { { 5, 5, 5, 5, 5 }, { 5, 5, 5, 5, 5 }, "All same elements" },

        // Test 8: With duplicates
        { { 3, 1, 4, 1, 5, 9, 2, 6, 5 }, { 1, 1, 2, 3, 4, 5, 5, 6, 9 }, "Array with duplicates" },

        // Test 9: Negative numbers
        { { -5, -1, -3, -2, -4 }, { -5, -4, -3, -2, -1 }, "All negative numbers" },

        // Test 10: Mixed positive and negative
        { { 3, -1, 4, -5, 2, 0, -3 }, { -5, -3, -1, 0, 2, 3, 4 }, "Mixed positive and negative" },

        // Test 11: With zeros
        { { 0, 5, 0, -2, 3, 0, 1 }, { -2, 0, 0, 0, 1, 3, 5 }, "Array with zeros" },

        // Test 12: Large range
        { { 100, -100, 50, -50, 0 }, { -100, -50, 0, 50, 100 }, "Large value range" },

        // Test 13: Mostly sorted
        { { 1, 2, 3, 5, 4, 6, 7 }, { 1, 2, 3, 4, 5, 6, 7 }, "Nearly sorted" },

        // Test 14: Reverse pairs
        { { 2, 1, 4, 3, 6, 5 }, { 1, 2, 3, 4, 5, 6 }, "Reverse pairs" },

        // Test 15: Three elements
        { { 3, 1, 2 }, { 1, 2, 3 }, "Three elements" },

        // Test 16: Random order
        { { 9, 3, 7, 1, 5, 2, 8, 4, 6 }, { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, "Random order" },

        // Test 17: Larger array
        {
            { 15, 3, 9, 8, 5, 2, 7, 1, 6, 4, 10, 12, 11, 14, 13 },
            { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, "Larger array"
        },

        // Test 18: Many duplicates
        { { 3, 1, 3, 2, 3, 1, 2 }, { 1, 1, 2, 2, 3, 3, 3 }, "Many duplicates" },

        // Test 19: Alternating high-low
        { { 10, 1, 9, 2, 8, 3, 7, 4 }, { 1, 2, 3, 4, 7, 8, 9, 10 }, "Alternating values" },

        // Test 20: Descending with duplicates
        { { 5, 5, 4, 4, 3, 3, 2, 2, 1, 1 }, { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 }, "Descending with duplicates" }
    };
}

TEST_P(SortAlgorithmTest, SortsCorrectly)
{
    auto [sort_func, test_case] = GetParam();
    std::vector<int> nums = test_case.input;
    sort_func(nums);
    EXPECT_EQ(nums, test_case.expected) << "Failed for: " << test_case.description;
}

// Instantiate tests for STL sort
INSTANTIATE_TEST_SUITE_P(
    STL_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::stl_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);

// Instantiate tests for Quick sort
INSTANTIATE_TEST_SUITE_P(
    Quick_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::quick_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);

// Instantiate tests for Merge sort
INSTANTIATE_TEST_SUITE_P(
    Merge_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::merge_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);

// Instantiate tests for Bubble sort
INSTANTIATE_TEST_SUITE_P(
    Bubble_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::bubble_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);

// Instantiate tests for Insertion sort
INSTANTIATE_TEST_SUITE_P(
    Insertion_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::insertion_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);

// Instantiate tests for Selection sort
INSTANTIATE_TEST_SUITE_P(
    Selection_Sort_Tests,
    SortAlgorithmTest,
    ::testing::Combine(
        ::testing::Values(std::function<void(std::vector<int>&)>(algo::selection_sort<int>)),
        ::testing::ValuesIn(GetTestCases())
    )
);
