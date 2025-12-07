#pragma once

namespace algo
{
    namespace local
    {
        template<class T>
        int quick_sort_partition(std::vector<T>& nums, int left, int right)
        {
            T pivot = nums[left + (right - left) / 2];

            int i = left, j = right;
            while (true)
            {
                while (nums[i] < pivot)
                    i++;
                while (nums[j] > pivot)
                    j--;

                if (i >= j)
                    break;

                std::swap(nums[i++], nums[j--]);
            }

            return j;
        }

        template<class T>
        void quick_sort(std::vector<T>& nums, int left, int right)
        {
            if (left < right)
            {
                int pivot = quick_sort_partition(nums, left, right);
                quick_sort(nums, left, pivot);
                quick_sort(nums, pivot + 1, right);
            }
        }

        template<class T>
        void merge_sort(std::vector<T>& nums, int left, int right)
        {
            if (left < right)
            {
                const T mid = left + (right - left) / 2;
                merge_sort(nums, left, mid);
                merge_sort(nums, mid + 1, right);

                std::vector<T> sorted;
                int i = left, j = mid + 1;
                while (i <= mid && j <= right)
                {
                    if (nums[i] < nums[j])
                        sorted.push_back(nums[i++]);
                    else
                        sorted.push_back(nums[j++]);
                }

                while (i <= mid)
                    sorted.push_back(nums[i++]);

                while (j <= right)
                    sorted.push_back(nums[j++]);

                for (size_t l = 0; l < sorted.size(); ++l)
                    nums[left + l] = sorted[l];
            }
        }

        template<class T>
        void bubble_sort(std::vector<T>& nums)
        {
            const int n = nums.size();
            for (int i = 0; i < n - 1; ++i)
            {
                bool swapped = false;
                for (int j = 0; j < n - 1 - i; ++j)
                {
                    if (nums[j + 1] < nums[j])
                    {
                        std::swap(nums[j], nums[j + 1]);
                        swapped = true;
                    }
                }

                if (!swapped)
                    break;
            }
        }

        template<class T>
        void insertion_sort(std::vector<T>& nums)
        {
            const int n = nums.size();
            for (int i = 1; i < n; ++i)
            {
                int j = i;
                while (j >= 1 && nums[j] < nums[j - 1])
                {
                    std::swap(nums[j], nums[j - 1]);
                    j--;
                }
            }
        }

        template<class T>
        void selection_sort(std::vector<T>& nums)
        {
            const int n = nums.size();
            for (int i = 0; i < n - 1; ++i)
            {
                int min_index = i;
                for (int j = i + 1; j < n; ++j)
                {
                    if (nums[j] < nums[min_index])
                        min_index = j;
                }
                std::swap(nums[i], nums[min_index]);
            }
        }
    }

    template<class T>
    void stl_sort(std::vector<T>& nums)
    {
        std::sort(nums.begin(), nums.end());
    }

    template<class T>
    void quick_sort(std::vector<T>& nums)
    {
        local::quick_sort(nums, 0, nums.size() - 1);
    }

    template<class T>
    void merge_sort(std::vector<T>& nums)
    {
        local::merge_sort(nums, 0, nums.size() - 1);
    }

    template<class T>
    void bubble_sort(std::vector<T>& nums)
    {
        local::bubble_sort(nums);
    }

    template<class T>
    void insertion_sort(std::vector<T>& nums)
    {
        local::insertion_sort(nums);
    }

    template<class T>
    void selection_sort(std::vector<T>& nums)
    {
        local::selection_sort(nums);
    }
}
