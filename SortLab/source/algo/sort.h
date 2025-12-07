#pragma once

#include <vector>

namespace algo
{
    template<class T>
    void stl_sort(std::vector<T>& range);

    template<class T>
    void quick_sort(std::vector<T>& nums);

    template<class T>
    void merge_sort(std::vector<T>& nums);

    template<class T>
    void bubble_sort(std::vector<T>& nums);

    template<class T>
    void insertion_sort(std::vector<T>& nums);

    template<class T>
    void selection_sort(std::vector<T>& nums);
}

#include "sort.inl"