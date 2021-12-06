#include <iostream>
#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
std::vector<std::vector<int>> paths;
std::vector<int> path;

void findPath(TreeNode* root, int targetSum) {
    if (root == nullptr) 
        return;

    path.push_back(root->val);

    if ((targetSum - root->val == 0) && (root->left == nullptr) && (root->right == nullptr)) {
        paths.push_back(path);
    }
    else {
        findPath(root->left, targetSum - root->val);
        findPath(root->right, targetSum - root->val);
    }
    path.pop_back();
}

std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) 
{
    findPath(root, targetSum);
    return paths;
}

int main(void)
{
    TreeNode* left_3 = new TreeNode(7);
    TreeNode* left_4 = new TreeNode(2);
    TreeNode* left_2 = new TreeNode(11, left_3, left_4);
    TreeNode* left_5 = new TreeNode(4, left_2, nullptr);

    TreeNode* right_2 = new TreeNode(1);
    TreeNode* right_3 = new TreeNode(5);
    TreeNode* right_4 = new TreeNode(4, right_2, right_3);
    TreeNode* right_5 = new TreeNode(13);

    TreeNode* left_1 = new TreeNode(4, left_2, nullptr);
    TreeNode* right_1 = new TreeNode(8, right_5, right_4);
    TreeNode* root = new TreeNode(5, left_1, right_1);

    std::vector<std::vector<int>> result = pathSum(root, 22);
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

	return 0;
}