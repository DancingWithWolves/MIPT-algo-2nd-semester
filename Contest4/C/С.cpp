#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>





template <typename T> 
class BinaryIndexedTree {
private:

    int AndFunc (int x) { return x & (x + 1); }
    int OrFunc (int x) { return x | (x + 1); }
    
	T* tree = nullptr;
	int size = 0;

public:
	BinaryIndexedTree (int n): size(n) { tree = new T[n](); }

	void Add (int index, T val) 
    {
		for (int i = index; i < size; i = OrFunc(i))
			tree[i] += val;
	}

	int Count (int index)
    {
		int ret = 0;

		for (int i = index; i >= 0; i = OrFunc(i) - 1)
			ret += tree[i];

		return ret;
	}

	T Count (int left, int right) {	return Count(right) - Count(left - 1); }
};

int main()
{
	/*std::cin.tie(nullptr);
	std::ios_base::sync_with_stdio(false);

	
	int n = 0;
	std::cin >> n;

    std::vector<int> data(n);
	for (int i = 0; i < n; ++i)
		std::cin >> data[i];


	std::vector<int> sorted (data.begin(), data.end());
    std::sort(sorted.begin(), sorted.end());

    std::map<int, int> sorted_pozition;
    for (int i = sorted.size() - 1; i >= 0; --i)
        sorted_pozition[sorted[i]] = i;

	for (int i = 0; i < data.size(); ++i)
        data[i] = sorted_pozition[data[i]];
    //теперь data -- перестановка индексов, на которых сответствующий элемент стоял бы в отсортированном массиве 
	std::vector<unsigned long long> less(data.size(), 0);
	BinaryIndexedTree<int> right_less(data.size());

    for (size_t i = data.size() - 1; i >= 0; --i) {

        less[i] = right_less.Count(data[i] - 1);
        right_less.Add(data[i], 1);

    }

	std::vector<unsigned long long> greater(data.size(), 0);
    BinaryIndexedTree<int> left_greater(data.size());

    for (size_t i = 0; i < data.size(); ++i) {

        greater[i] = left_greater.Count(data[i] + 1, data.size() - 1);
		left_greater.Add(data[i], 1);

    }

	unsigned long long ans = 0;
    for (size_t i = 0; i < data.size(); ++i){
        ans += less[i] * greater[i];
	}

	std::cout << ans;

	return 0;*/

    std::cin.tie(nullptr);
	std::ios_base::sync_with_stdio(false);

	std::vector<int> data;
	int n;

	std::cin >> n;
	int val = 0;

	for (int i = 0; i < n; ++i) {
		std::cin >> val;
		data.push_back(val);
	}

	std::vector<int> s(data.begin(), data.end());
    std::sort(s.begin(), s.end());

    std::map<int, int> m;
    for (int i = s.size() - 1; i >= 0; --i)
        m[s[i]] = i;

	for (int i = 0; i < data.size(); ++i)
        data[i] = m[data[i]];

	std::vector<unsigned long long> less(data.size(), 0);
	BinaryIndexedTree<int> nl(data.size());

    for (int i = data.size() - 1; i >= 0; --i) {

        less[i] = nl.Count(data[i] - 1);
        nl.Add(data[i], 1);
    }

	std::vector<unsigned long long> greater(data.size(), 0);
    BinaryIndexedTree<int> pg(data.size());

    for (size_t i = 0; i < data.size(); ++i) {

        greater[i] = pg.Count(data[i] + 1, data.size() - 1);
		pg.Count(data[i], 1);
    }

	unsigned long long ans = 0;
    for (size_t i = 0; i < data.size(); ++i){
        ans += less[i] * greater[i];
	}

	std::cout << ans;

	return 0;
}