#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(NULL) {}
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
	ListNode* dummy = new ListNode(0);
	ListNode* p = dummy;
	while (1) {
		int min = -1;
		for (int i = 0; i < lists.size(); ++i)
			if (lists[i] != NULL) {
				min = i;
				break;
			}
		if (min == -1) 
			break;
		for (int i = 0; i < lists.size(); ++i)//find the min value
			if (lists[i] != NULL)
				if (lists[i]->val < lists[min]->val) 
					min = i;
		//point to next
		dummy->next = lists[min];
		dummy = dummy->next;
		lists[min] = lists[min]->next;
	}
	return p->next;
}

int main(void)
{
	ListNode* first;
	first->val = 1;
	first->next = new ListNode(4);
	first->next->next = new ListNode(5);

	ListNode* second;
	first->val = 1;
	first->next = new ListNode(3);
	first->next->next = new ListNode(4);

	ListNode* third;
	first->val = 2;
	first->next = new ListNode(6);

	vector<ListNode*> input;
	input.push_back(first);
	input.push_back(second);
	input.push_back(third);

	ListNode* result = mergeKLists(input);

	return 0;
}
