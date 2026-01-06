#pragma once

namespace algo
{
    template<class RangeType>
    void bubble_sort(RangeType&& range);

    template<class RangeType>
    void insertion_sort(RangeType&& range);

    template<class RangeType>
    void selection_sort(RangeType&& range);

    template<class RangeType>
    void stl_sort(RangeType&& range);

    template<class RangeType>
    void boost_spread_sort(RangeType&& range);

    template<class RangeType>
    void boost_spin_sort(RangeType&& range);

    template<class RangeType>
    void quick_sort(RangeType&& range);

    template<class RangeType>
    void merge_sort(RangeType&& range);

    namespace concurrent
    {
        template<class RangeType>
        void merge_sort(RangeType&& range);

        template<class RangeType>
        void merge_sort_advanced(RangeType&& range);
    }
}

#include "sort.inl"
