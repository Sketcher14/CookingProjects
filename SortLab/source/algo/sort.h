#pragma once

#include <concepts>
#include <ranges>

namespace algo
{
    /** Elements must be comparable and movable for sorting */
    template<typename T>
    concept Sortable = std::totally_ordered<T> && std::movable<T>;

    /** Sortable range with contiguous memory (supports .data() and .size()) */
    template<typename RangeType>
    concept ContiguousSortableRange =
            std::ranges::contiguous_range<RangeType> &&
            std::ranges::sized_range<RangeType> &&
            Sortable<std::ranges::range_value_t<RangeType>>;

    /** Sortable range with random access iterators */
    template<typename RangeType>
    concept RandomAccessSortableRange =
            std::ranges::random_access_range<RangeType> &&
            Sortable<std::ranges::range_value_t<RangeType>>;

    template<ContiguousSortableRange RangeType>
    void bubble_sort(RangeType&& range);

    template<ContiguousSortableRange RangeType>
    void insertion_sort(RangeType&& range);

    template<ContiguousSortableRange RangeType>
    void selection_sort(RangeType&& range);

    template<RandomAccessSortableRange RangeType>
    void stl_sort(RangeType&& range);

    template<RandomAccessSortableRange RangeType>
    void boost_spread_sort(RangeType&& range);

    template<RandomAccessSortableRange RangeType>
    void boost_spin_sort(RangeType&& range);

    template<ContiguousSortableRange RangeType>
    void quick_sort(RangeType&& range);

    template<ContiguousSortableRange RangeType>
    void merge_sort(RangeType&& range);

    namespace concurrent
    {
        template<ContiguousSortableRange RangeType>
        void merge_sort(RangeType&& range);

        template<ContiguousSortableRange RangeType>
        void merge_sort_advanced(RangeType&& range);
    }
}

#include "sort.inl"
