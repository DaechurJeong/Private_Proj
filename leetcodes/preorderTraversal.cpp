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

std::vector<int> preorderTraversal(TreeNode* root) {
    std::vector<int> ret;
    std::stack<TreeNode*> st;

    while (root || !st.empty()) {
        if (root) {
            ret.push_back(root->val);
            if (root->right) {
                st.push(root->right);
            }
            root = root->left;
        }
        else {
            root = st.top();
            st.pop();
        }
    }
    return ret;
}

int main(void) {
    TreeNode* root = new TreeNode(1);
    root->left = nullptr;
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);

    std::vector<int> output = preorderTraversal(root);

    for (auto it : output) {
        std::cout << it << ", ";
    }
    std::cout << std::endl;

    return 0;
}