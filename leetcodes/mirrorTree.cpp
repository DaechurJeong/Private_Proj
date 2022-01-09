#include <iostream>

struct Node
{
    int data;
    struct Node* left;
    struct Node* right;

    Node(int x){
        data = x;
        left = right = NULL;
    }
};

void mirror(Node* node) {
    // code here
    if(!node) return;
    mirror(node->left);
    mirror(node->right);
    Node* tmp = node->left;
    node->left = node->right;
    node->right = tmp;
}

int main(void)
{
	Node* root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);

	mirror(root);

	return 0;
}