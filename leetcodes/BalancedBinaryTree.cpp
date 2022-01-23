#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

int getHeight(TreeNode* root) {
    return (root == nullptr) ? 0 : max(getHeight(root->left), getHeight(root->right)) + 1;
}
bool isBalanced(TreeNode* root) {
    if (root == nullptr) return true;
    if (!isBalanced(root->left) || !isBalanced(root->right)) {
        return false;
    }
    return abs(getHeight(root->left) - getHeight(root->right)) <= 1;
}

int main(void)
{

    TreeNode* four1 = new TreeNode(4);
    TreeNode* four2 = new TreeNode(4);

    TreeNode* three1 = new TreeNode(3, four1, four2);
    TreeNode* three2 = new TreeNode(3);

    TreeNode* two1 = new TreeNode(2, three1, three2);
    TreeNode* two2 = new TreeNode(2);

    TreeNode* root = new TreeNode(1, two1, two2);

    string output = isBalanced(root) ? "TRUE" : "FALSE";
    cout << output << endl;
	return 0;
}