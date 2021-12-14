#include <iostream>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

/*int lc, rc;
int helper(TreeNode* root, int x) {
    if (root == NULL) {
        return 0;
    }
    int l = helper(root->left, x);
    int r = helper(root->right, x);
    if (root->val == x) {
        lc = l;
        rc = r;
    }
    return l + r + 1;
}
bool btreeGameWinningMove(TreeNode* root, int n, int x) {
    lc = 0, rc = 0;
    helper(root, x);
    int k = n - lc - rc - 1;
    int z = std::max({k, lc, rc});
    return (z > n / 2);
}*/
TreeNode* RED = nullptr;
int dfs(TreeNode* node, int x) {
    // Reached a leaf node
    if (node == nullptr) return 0;

    // Reached the RED node
    if (node->val == x) {
        RED = node;
        return 0;
    }
    return 1 + dfs(node->left, x) + dfs(node->right, x);
}
bool btreeGameWinningMove(TreeNode* root, int n, int x) {
    // Boundary case
    if (n <= 1) return false;

    // Number of blue nodes based on move
    int numBlueNodes = 0;

    // Initialize 'RED' node
    RED = root;

    // Choose the parent node (if available)
    if (root->val != x) {
        numBlueNodes = dfs(root, x);
    }
    if (numBlueNodes > (n / 2)) return true;

    // Choose the right child node (if available)
    numBlueNodes = dfs(RED->right, x);
    if (numBlueNodes > (n / 2)) return true;

    // Choose the left child node (if available)
    numBlueNodes = dfs(RED->left, x);
    if (numBlueNodes > (n / 2)) return true;

    // No winning moves
    return false;
}

int main(void) {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    root->left->left->left = new TreeNode(8);
    root->left->left->right = new TreeNode(9);

    root->left->right->left = new TreeNode(10);
    root->left->right->right = new TreeNode(11);

    std::string ans = btreeGameWinningMove(root, 11, 3) ? "TRUE" : "FALSE";
    //std::string ans = btreeGameWinningMove(root, 3, 1) ? "TRUE" : "FALSE";

    std::cout << ans << std::endl;

	return 0;
}