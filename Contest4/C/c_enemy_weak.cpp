#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>

template <class T> 
class BinaryIndexedTree {
private:

	int AndFunc (int x) { return x & (x + 1); }
    int OrFunc (int x) { return x | (x + 1); }
    
	T* tree = nullptr;
	int size = 0;

public:
	BinaryIndexedTree(int n): size(n) { tree = new T[n](); }
    ~BinaryIndexedTree() { delete[] tree; }

	void Add (int index, T val)
    {
		for (int i = index; i < size; i = OrFunc(i))
			tree[i] += val;
	}

	int Count (int index)
    {
		int ret = 0;
		for (int i = index; i >= 0; i = AndFunc(i) - 1)
			ret += tree[i]; 
		return ret;
	}

	T Count (int left, int right) { return Count(right) - Count(left - 1); }
};

int main()
{
	int n = 0;
	scanf("%d", &n);

    std::vector<int> data(n);

	for (int i = 0; i < n; ++i)
		scanf("%d", &data[i]);

	std::vector<int> sorted(data.begin(), data.end());
    std::sort(sorted.begin(), sorted.end());

    std::map<int, int> sortedIndexes;
    for (int i = sorted.size() - 1; i >= 0; --i)
        sortedIndexes[sorted[i]] = i;

	for (int i = 0; i < n; ++i)
        data[i] = sortedIndexes[data[i]];

    //Теперь в data хранится перестановка индексов

	std::vector<unsigned long long> right_less(n);
	BinaryIndexedTree<int> less(n);

    for (int i = n - 1; i >= 0; --i) {
        right_less[i] = less.Count(data[i] - 1);
        less.Add(data[i], 1);
    }

	std::vector<unsigned long long> left_greater(n);
    BinaryIndexedTree<int> greater(n);

    for (size_t i = 0; i < n; ++i) {

        left_greater[i] = greater.Count(data[i] + 1, n - 1);
		greater.Add(data[i], 1);
    }

	unsigned long long ans = 0;
    for (size_t i = 0; i < n; ++i){
        ans += right_less[i] * left_greater[i];
	}

	printf("%llu\n", ans);

	return 0;
}