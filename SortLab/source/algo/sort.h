#pragma once

#include <vector>

namespace algo
{
    template<class RangeType>
    void stl_sort(RangeType&& range);

    template<class RangeType>
    void quick_sort(RangeType&& range);

    template<class RangeType>
    void merge_sort(RangeType&& range);

    template<class RangeType>
    void bubble_sort(RangeType&& range);

    template<class RangeType>
    void insertion_sort(RangeType&& range);

    template<class RangeType>
    void selection_sort(RangeType&& range);
}

#include "sort.inl"