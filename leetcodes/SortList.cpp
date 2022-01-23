#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
    
};

ListNode* middleNode(ListNode* head) {
    ListNode* midPrev = nullptr;
    while (head && head->next) {
        midPrev = (midPrev == nullptr) ? head : midPrev->next;
        head = head->next->next;
    }
    ListNode* mid = midPrev->next;
    midPrev->next = nullptr;
    return mid;
}

ListNode* merge(ListNode* list1, ListNode* list2)
{
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    ListNode* newNode;
    if (list1->val <= list2->val) {
        newNode = list1;
        newNode->next = merge(list1->next, list2);
    }
    else {
        newNode = list2;
        newNode->next = merge(list1, list2->next);
    }
    return newNode;
}
ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode* mid = middleNode(head);
    ListNode* left = sortList(head);
    ListNode* right = sortList(mid);
    return merge(left, right);
}
int main(void)
{
    ListNode* head = new ListNode(4);
    head->next = new ListNode(2);
    head->next->next = new ListNode(1);
    head->next->next->next = new ListNode(3);
    ListNode* sorted = sortList(head);

	return 0;
}