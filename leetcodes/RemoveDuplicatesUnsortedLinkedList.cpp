#include <iostream>
#include <set>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* deleteDuplicates(ListNode* head)
{
    if (head == nullptr) return nullptr;
    set<int> values;
    set<int> toDelete;

    ListNode* curr = head;
    while (curr != nullptr) {
        int num = curr->val;
        if (values.find(num) == values.end()) {
            values.emplace(curr->val);
        }
        else {
            toDelete.emplace(curr->val);
        }
        curr = curr->next;
    }
    curr = head;
    ListNode* res = new ListNode(0);
    ListNode* currHead = res;
    while (curr != nullptr) {
        if (toDelete.find(curr->val) == toDelete.end()) {
            res->next = new ListNode(curr->val);
            res = res->next;
        }
        curr = curr->next;
    }
    return currHead->next;
}
int main(void)
{
    ListNode* root = new ListNode(1);
    root->next = new ListNode(2);
    root->next->next = new ListNode(3);
    root->next->next->next = new ListNode(2);

    ListNode* output = deleteDuplicates(root);

    while (output != nullptr) {
        cout << output->val << endl;
        output = output->next;
    }

	return 0;
}