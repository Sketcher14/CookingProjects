#pragma once

#include <boost/sort/spreadsort/spreadsort.hpp>
#include <boost/sort/spinsort/spinsort.hpp>
#include <algorithm>
#include <cassert>
#include <future>
#include <vector>

namespace algo
{
    template<class T>
    size_t binary_search(const T* range, const size_t size, const T target)
    {
        size_t left = 0, right = size;

        while (left < right)
        {
            const size_t mid = left + (right - left) / 2;
            if (target == range[mid])
                return mid;
            if (target < range[mid])
                right = mid;
            else
                left = mid + 1;
        }

        return left;
    }

    namespace local
    {
        template<class T>
        void bubble_sort(T* range, const size_t size)
        {
            for (size_t i = 0; i + 1 < size; ++i)
            {
                bool swapped = false;
                for (size_t j = 0; j + i + 1 < size; ++j)
                {
                    if (range[j + 1] < range[j])
                    {
                        std::swap(range[j], range[j + 1]);
                        swapped = true;
                    }
                }

                if (!swapped)
                    break;
            }
        }

        template<class T>
        void insertion_sort(T* range, const size_t size)
        {
            for (size_t i = 1; i < size; ++i)
            {
                size_t j = i;
                T key = range[i];
                while (j >= 1 && key < range[j - 1])
                {
                    range[j] = range[j - 1];
                    j--;
                }
                range[j] = key;
            }
        }

        template<class T>
        void selection_sort(T* range, const size_t size)
        {
            for (size_t i = 0; i + 1 < size; ++i)
            {
                size_t min_index = i;
                for (size_t j = i + 1; j < size; ++j)
                {
                    if (range[j] < range[min_index])
                        min_index = j;
                }
                std::swap(range[i], range[min_index]);
            }
        }

        template<class T>
        size_t quick_sort_partition(T* range, const size_t size)
        {
            assert(size > 1);

            // TODO Improve selecting the pivot element
            T pivot = range[(size - 1) / 2];

            size_t i = 0, j = size - 1;
            while (true)
            {
                while (range[i] < pivot)
                    i++;
                while (range[j] > pivot)
                    j--;

                if (i >= j)
                    break;

                std::swap(range[i++], range[j--]);
            }

            return j;
        }

        template<class T>
        void quick_sort(T* range, const size_t size)
        {
            if (size <= 1)
                return;

            size_t pivot = quick_sort_partition(range, size);
            quick_sort(range, pivot + 1);
            quick_sort(range + pivot + 1, size - pivot - 1);
        }

        template<class T>
        void merge(T* range, const size_t left1, const size_t right1, const size_t left2, const size_t right2,
                   std::vector<T>& merge_result, const size_t padding)
        {
            assert(left1 <= right1);
            assert(left2 <= right2);

            const size_t size1 = right1 - left1, size2 = right2 - left2;
            size_t i = 0, j = 0;

            while (i < size1 && j < size2)
            {
                if (range[left1 + i] < range[left2 + j])
                {
                    merge_result[padding + i + j] = range[left1 + i];
                    i++;
                }
                else
                {
                    merge_result[padding + i + j] = range[left2 + j];
                    j++;
                }
            }

            while (i < size1)
            {
                merge_result[padding + i + j] = range[left1 + i];
                i++;
            }

            while (j < size2)
            {
                merge_result[padding + i + j] = range[left2 + j];
                j++;
            }
        }

        template<class T>
        void merge_sort(T* range, const size_t size)
        {
            if (size <= 1)
                return;

            const size_t mid = size / 2;
            merge_sort(range, mid);
            merge_sort(range + mid, size - mid);

            // Merging left [0, mid) and right [mid, size) parts and then copying it to the initial range
            // TODO Can avoid the allocation for every recursion call?
            std::vector<T> merge_result(size);
            merge(range, 0, mid, mid, size, merge_result, 0);
            std::copy(merge_result.begin(), merge_result.end(), range);
        }

        namespace concurrent
        {
            static constexpr size_t min_size_for_threading = 1000;


            template<class T>
            void copy(T* range, const size_t size, T* result)
            {
                if (size <= min_size_for_threading)
                {
                    std::copy(range, range + size, result);
                    return;
                }

                const size_t mid = size / 2;

                // This thread copies the left part, a new thread copies the right part
                auto right_future = std::async(std::launch::async, copy<T>, range + mid, size - mid, result + mid);
                copy(range, mid, result);

                // Waiting for the right part to be copied
                right_future.get();
            }

            template<class T>
            void merge_sort(T* range, const size_t size)
            {
                if (size <= 1)
                    return;

                if (size <= min_size_for_threading)
                {
                    local::merge_sort(range, size);
                    return;
                }

                const size_t mid = size / 2;
                // This thread sorts the left part, a new thread sorts the right part
                auto right_future = std::async(std::launch::async, concurrent::merge_sort<T>, range + mid, size - mid);
                concurrent::merge_sort(range, mid);

                // Waiting for the right part to be sorted
                right_future.get();

                // One threaded merging of left [0, mid) and right [mid, size) parts
                // and then copying it to the initial range
                std::vector<T> merge_result(size);
                local::merge(range, 0, mid, mid, size, merge_result, 0);
                std::copy(merge_result.begin(), merge_result.end(), range);
            }

            template<class T>
            void merge_advanced(T* range, size_t left1, size_t right1, size_t left2, size_t right2,
                                std::vector<T>& merge_result, const size_t padding)
            {
                if (left1 >= right1 && left2 >= right2)
                    return;

                assert(left1 <= right1);
                assert(left2 <= right2);

                const size_t size1 = right1 - left1, size2 = right2 - left2;

                if (size1 + size2 <= min_size_for_threading)
                {
                    local::merge(range, left1, right1, left2, right2, merge_result, padding);
                    return;
                }

                // The first range must be always larger than the second one
                if (size2 > size1)
                {
                    std::swap(left1, left2);
                    std::swap(right1, right2);
                }

                const size_t mid1 = left1 + (right1 - left1) / 2;
                const T median1 = range[mid1];
                const size_t mid2 = left2 + algo::binary_search(range + left2, right2 - left2, median1);
                const size_t median1_index = padding + (mid1 - left1) + (mid2 - left2);
                merge_result[median1_index] = median1;

                // This thread merges left parts from the first and the second ranges,
                // a new thread merges right parts from the first and the second ranges
                auto right_future = std::async(std::launch::async, concurrent::merge_advanced<T>,
                                               range, mid1 + 1, right1, mid2, right2, std::ref(merge_result),
                                               median1_index + 1);
                concurrent::merge_advanced(range, left1, mid1, left2, mid2, merge_result, padding);

                // Waiting for the right parts to be merged
                right_future.get();
            }

            template<class T>
            void merge_sort_advanced(T* range, const size_t size)
            {
                if (size <= 1)
                    return;

                if (size <= min_size_for_threading)
                {
                    local::merge_sort(range, size);
                    return;
                }

                const size_t mid = size / 2;
                // This thread sorts the left part, a new thread sorts the right part
                auto right_future = std::async(std::launch::async, concurrent::merge_sort_advanced<T>,
                                               range + mid, size - mid);
                concurrent::merge_sort_advanced(range, mid);

                // Waiting for the right part to be sorted
                right_future.get();

                // Multithreaded merging of left [0, mid) and right [mid, size) parts
                // and then copying it to the initial range
                std::vector<T> merge_result(size);
                concurrent::merge_advanced(range, 0, mid, mid, size, merge_result, 0);
                concurrent::copy(merge_result.data(), merge_result.size(), range);
            }
        }
    }

    template<class RangeType>
    void bubble_sort(RangeType&& range)
    {
        local::bubble_sort(range.data(), range.size());
    }

    template<class RangeType>
    void insertion_sort(RangeType&& range)
    {
        local::insertion_sort(range.data(), range.size());
    }

    template<class RangeType>
    void selection_sort(RangeType&& range)
    {
        local::selection_sort(range.data(), range.size());
    }

    template<class RangeType>
    void stl_sort(RangeType&& range)
    {
        std::sort(range.begin(), range.end());
    }

    template<class RangeType>
    void boost_spread_sort(RangeType&& range)
    {
        boost::sort::spreadsort::spreadsort(range.begin(), range.end());
    }

    template<class RangeType>
    void boost_spin_sort(RangeType&& range)
    {
        boost::sort::spinsort(range.begin(), range.end());
    }

    template<class RangeType>
    void quick_sort(RangeType&& range)
    {
        local::quick_sort(range.data(), range.size());
    }

    template<class RangeType>
    void merge_sort(RangeType&& range)
    {
        local::merge_sort(range.data(), range.size());
    }

    namespace concurrent
    {
        template<class RangeType>
        void merge_sort(RangeType&& range)
        {
            local::concurrent::merge_sort(range.data(), range.size());
        }

        template<class RangeType>
        void merge_sort_advanced(RangeType&& range)
        {
            local::concurrent::merge_sort_advanced(range.data(), range.size());
        }
    }
}
