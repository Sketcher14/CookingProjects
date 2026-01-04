#pragma once

#include <boost/sort/spreadsort/spreadsort.hpp>
#include <boost/sort/spinsort/spinsort.hpp>
#include <cassert>
#include <algorithm>
#include <vector>

namespace algo
{
    namespace local
    {
        template<class T>
        size_t quick_sort_partition(T* first, const size_t num)
        {
            assert(num > 1);

            T pivot = first[(num - 1) / 2];

            size_t i = 0, j = num - 1;
            while (true)
            {
                while (first[i] < pivot)
                    i++;
                while (first[j] > pivot)
                    j--;

                if (i >= j)
                    break;

                std::swap(first[i++], first[j--]);
            }

            return j;
        }

        template<class T>
        void quick_sort(T* first, const size_t num)
        {
            if (num > 1)
            {
                size_t pivot = quick_sort_partition(first, num);
                quick_sort(first, pivot + 1);
                quick_sort(first + pivot + 1, num - pivot - 1);
            }
        }

        template<class T>
        void merge_sort(T* first, const size_t num)
        {
            if (num > 1)
            {
                const size_t mid = num / 2;
                merge_sort(first, mid);
                merge_sort(first + mid, num - mid);

                std::vector<T> temp;
                size_t i = 0, j = mid;
                while (i < mid && j < num)
                {
                    if (first[i] < first[j])
                        temp.push_back(first[i++]);
                    else
                        temp.push_back(first[j++]);
                }

                while (i < mid)
                    temp.push_back(first[i++]);

                while (j < num)
                    temp.push_back(first[j++]);

                std::copy(temp.begin(), temp.end(), first);
            }
        }

        template<class T>
        void bubble_sort(T* first, const size_t num)
        {
            for (size_t i = 0; i + 1 < num; ++i)
            {
                bool swapped = false;
                for (size_t j = 0; j + i + 1 < num; ++j)
                {
                    if (first[j + 1] < first[j])
                    {
                        std::swap(first[j], first[j + 1]);
                        swapped = true;
                    }
                }

                if (!swapped)
                    break;
            }
        }

        template<class T>
        void insertion_sort(T* first, const size_t num)
        {
            for (size_t i = 1; i < num; ++i)
            {
                size_t j = i;
                while (j >= 1 && first[j] < first[j - 1])
                {
                    std::swap(first[j], first[j - 1]);
                    j--;
                }
            }
        }

        template<class T>
        void selection_sort(T* first, const size_t num)
        {
            for (size_t i = 0; i + 1 < num; ++i)
            {
                size_t min_index = i;
                for (size_t j = i + 1; j < num; ++j)
                {
                    if (first[j] < first[min_index])
                        min_index = j;
                }
                std::swap(first[i], first[min_index]);
            }
        }
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
    void boost_spreadsort(RangeType&& range)
    {
        boost::sort::spreadsort::spreadsort(range.begin(), range.end());
    }

    template<class RangeType>
    void boost_spinsort(RangeType&& range)
    {
        boost::sort::spinsort(range.begin(), range.end());
    }
}
