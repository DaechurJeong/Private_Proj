#include <iostream>
#include <vector>
#include <stack>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}

};

std::vector<int> inorderTraversal(TreeNode* root) {
    std::vector<int> ret;
    std::stack<TreeNode*> st;

    while (root || !st.empty()) {
        while (root) {
            st.push(root);
            root = root->left;
        }
        root = st.top();
        st.pop();
        ret.push_back(root->val);
        root = root->right;
    }
    return ret;
}

int main(void) {
    TreeNode* root = new TreeNode(1);
    root->left = nullptr;
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);

    std::vector<int> output = inorderTraversal(root);

    for (auto it : output) {
        std::cout << it << ", ";
    }
    std::cout << std::endl;

    return 0;
}