#include <vector>
class Solution
{
public:
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2)
    {

        std::vector<int> merged;
        int i = 0, j = 0;

        while (i < nums1.size() && j < nums2.size())
        {
            if (nums1[i] < nums2[j])
            {
                merged.push_back(nums1[i++]);
            }
            else
            {
                merged.push_back(nums2[j++]);
            }
        }
        while (i < nums1.size())
        {
            merged.push_back(nums1[i++]);
        }
        while (j < nums2.size())
        {
            merged.push_back(nums2[j++]);
        }
        int s = merged.size();
        if (s % 2 == 0)
        {
            return ((merged[(s / 2)] + merged[(s / 2) - 1]) / 2.000);
        }
        return merged[s / 2];
    }
};