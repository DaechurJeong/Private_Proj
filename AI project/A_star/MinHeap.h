#pragma once

template<typename GHeap, typename KeyHash, typename Compare>
class GenericMinHeap
{
private:
	std::vector<GHeap> element;
	std::unordered_map<intptr_t, unsigned int> hash_table;
public:
	GenericMinHeap();
	~GenericMinHeap();
	
	void MinHeapify(int index);
	void ReverseHeapify(int index);
	void decreaseKey(GHeap new_val);

	GHeap extractMin();
	
	void insertKey(GHeap val);

	void swap(GHeap* x, GHeap* y);
	void clear();
	void reserve(unsigned size);
	void pop();
	bool empty();
	// Gettor
	int getMin() { return element[0]; }
	size_t getSize() { return element.size(); }
	int getParent(int index) { return (index - 1) / 2; }
	int getLeft(int index) { return (2 * index) + 1; }
	int getRight(int index) { return (2 * index) + 2; }
};

template<typename GHeap, typename KeyHash, typename Compare>
GenericMinHeap<GHeap, typename KeyHash, typename Compare>::GenericMinHeap()
{}
template<typename GHeap, typename KeyHash, typename Compare>
GenericMinHeap<GHeap, typename KeyHash, typename Compare>::~GenericMinHeap()
{}

template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::MinHeapify(int index)
{
	KeyHash hashkey;
	Compare compare_key;
	int l = getLeft(index);
	int r = getRight(index);
	int min;

	// childs are exist?
	if (l >= element.size())
		return;
	else if (r >= element.size())
		min = l;
	else if (!compare_key(element[l], element[r])) // find smallest one
		min = l;
	else
		min = r;

	if (!compare_key(element[min], element[index]))
	{
		swap(&element[index], &element[min]); // go downward
		hash_table[hashkey(element[min])] = min;
		hash_table[hashkey(element[index])] = index;

		// Recursion call until top of elem is reached
		MinHeapify(min);
	}
}
template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::ReverseHeapify(int index)
{
	if (!index)
		return;
	KeyHash hashkey;
	Compare compare_key;

	int parent_ = getParent(index);

	if (compare_key(element[parent_], element[index]))
	{
		swap(&element[index], &element[parent_]);

		hash_table[hashkey(element[parent_])] = parent_;
		hash_table[hashkey(element[index])] = index;

		// Recursion call until top of elem is reached
		ReverseHeapify(parent_);
	}
}
template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::decreaseKey(GHeap new_val)
{
	KeyHash hash_key;

	element[hash_table[hash_key(new_val)]] = new_val;
	ReverseHeapify(hash_table[hash_key(new_val)]);
}

template<typename GHeap, typename KeyHash, typename Compare>
GHeap GenericMinHeap<GHeap, typename KeyHash, typename Compare>::extractMin()
{
	if (!element.empty())
		return element[0];

	return GHeap(); // return it
}

template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::insertKey(GHeap new_val)
{
	KeyHash hash_key;

	// Is the key in the table already?
	if (hash_table.find(hash_key(new_val)) != hash_table.end())
		return;
	
	// Indes is the end of heap
	hash_table[hash_key(new_val)] = unsigned int(element.size());
	element.push_back(new_val);
	ReverseHeapify(hash_table[hash_key(new_val)]);
}
template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::pop()
{
	if (!element.size())
		return;
	KeyHash hash_key;

	// Replace top element with the last
	GHeap top = element[0];
	element[0] = element[element.size() - 1];
	hash_table[hash_key(element[0])] = 0;

	element.pop_back();
	hash_table.erase(hash_key(top));

	MinHeapify(0);
}
template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::swap(GHeap * x, GHeap * y)
{
	GHeap temp = *x;
	*x = *y;
	*y = temp;
}

template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::clear()
{
	element.clear();
	hash_table.clear();
}

template<typename GHeap, typename KeyHash, typename Compare>
void GenericMinHeap<GHeap, typename KeyHash, typename Compare>::reserve(unsigned size)
{
	element.reserve(size);
	hash_table.reserve(size);
}

template<typename GHeap, typename KeyHash, typename Compare>
bool GenericMinHeap<GHeap, typename KeyHash, typename Compare>::empty()
{
	return element.empty();
}