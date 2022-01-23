#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
    
};

ListNode* deleteDuplicates(ListNode* head) {
    ListNode* curr = head;
    ListNode* ret = curr;
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->val == curr->next->val) {
            ListNode* nxt = curr->next;
            while (nxt != nullptr) {
                if (nxt->val != curr->val) {
                    break;
                }
                nxt = nxt->next;
            }
            curr->next = nxt;
        }
        curr = curr->next;
    }
    return ret;
}

int main(void)
{
    ListNode* head = new ListNode(1);
    head->next = new ListNode(1);
    head->next->next = new ListNode(2);
    head->next->next->next = new ListNode(3);
    head->next->next->next->next = new ListNode(3);

    ListNode* sorted = deleteDuplicates(head);

	return 0;
}