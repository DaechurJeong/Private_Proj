#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(NULL) {}
};

// num24
ListNode* swapPairs(ListNode* head) {
	ListNode* dummyHead = new ListNode(0);
	dummyHead->next = head;

	ListNode* p = dummyHead;
	while (p->next && p->next->next) {

		ListNode* node1 = p->next;
		ListNode* node2 = node1->next;
		ListNode* next = node2->next;

		node2->next = node1;
		node1->next = next;
		p->next = node2;

		p = node1;
	}

	ListNode* retNode = dummyHead->next;
	delete dummyHead;

	return retNode;
}

// num25
ListNode* reverseKGroup(ListNode* head, int k) {
	if (head == nullptr || head->next == nullptr || k < 2)
		return head;
	int cnt = 1;
	ListNode* p = head;
	while (cnt != k)
	{
		p = p->next;
		if (p == nullptr)
			return head;
		++cnt;
	}
	ListNode* next = p->next;
	p->next = nullptr;
	ListNode* p1 = head;
	ListNode* p2 = head->next;
	p1->next = nullptr;
	while (p2)
	{
		ListNode* temp = p2->next;
		p2->next = p1;
		p1 = p2;
		p2 = temp;
	}
	head->next = reverseKGroup(next, k);
	return p1;
}
int main(void)
{
	ListNode* first = new ListNode(1);
	first->next = new ListNode(2);
	first->next->next = new ListNode(3);
	first->next->next->next = new ListNode(4);
	first->next->next->next->next = new ListNode(5);

	int count = 3;

	ListNode* output = reverseKGroup(first, count);

	return 0;
}
