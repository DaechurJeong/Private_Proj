#include <iostream>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
TreeNode* buildTree(vector<int>& nums, int left, int right)
{
    if (left > right) return nullptr;
    int mid = left + (right - left) / 2;
    TreeNode* root = new TreeNode(nums[mid]);
    root->left = buildTree(nums, left, mid-1);
    root->right = buildTree(nums, mid + 1, right);
    return root;
}
TreeNode* sortedArrayToBST(vector<int>& nums) {
    return buildTree(nums, 0, nums.size() - 1);
}

int main(void)
{
    vector<int> input{ -10,-3,0,5,9 };

    TreeNode* output = sortedArrayToBST(input);

	return 0;
}