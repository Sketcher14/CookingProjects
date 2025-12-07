#include "sort.h"

namespace algo
{
    namespace local
    {
        int quick_sort_partition(std::vector<int>& nums, int left, int right)
        {
            int pivot = nums[left + (right - left) / 2];

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

        void quick_sort(std::vector<int>& nums, int left, int right)
        {
            if (left < right)
            {
                int pivot = quick_sort_partition(nums, left, right);
                quick_sort(nums, left, pivot);
                quick_sort(nums, pivot + 1, right);
            }
        }

        void merge_sort(std::vector<int>& nums, int left, int right)
        {
            if (left < right)
            {
                const int mid = left + (right - left) / 2;
                merge_sort(nums, left, mid);
                merge_sort(nums,mid + 1, right);

                std::vector<int> sorted;
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

        void bubble_sort(std::vector<int>& nums)
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

        void insertion_sort(std::vector<int>& nums)
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

        void selection_sort(std::vector<int>& nums)
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

    void stl_sort(std::vector<int>& nums)
    {
        std::sort(nums.begin(), nums.end());
    }

    void quick_sort(std::vector<int>& nums)
    {
        local::quick_sort(nums, 0, nums.size() - 1);
    }

    void merge_sort(std::vector<int>& nums)
    {
        local::merge_sort(nums, 0, nums.size() - 1);
    }

    void bubble_sort(std::vector<int>& nums)
    {
        local::bubble_sort(nums);
    }

    void insertion_sort(std::vector<int>& nums)
    {
        local::insertion_sort(nums);
    }

    void selection_sort(std::vector<int>& nums)
    {
        local::selection_sort(nums);
    }
}
