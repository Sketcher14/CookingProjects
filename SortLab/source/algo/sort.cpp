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
            // iterative
            if (left < right)
            {

            }
        }

        void bubble_sort(std::vector<int>& nums)
        {
            int n = nums.size();
            for (int i = 0; i < n - 1; i++)
            {
                bool swapped = false;
                for (int j = 0; j < n - i - 1; j++)
                {
                    if (nums[j] > nums[j + 1])
                    {
                        std::swap(nums[j], nums[j + 1]);
                        swapped = true;
                    }
                }
                if (!swapped)
                    break;
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
}
