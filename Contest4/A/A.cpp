#include <iostream>
#include <stdio.h>

 
template <class T>
 
class BinaryIndexedTree {
public:
 
	BinaryIndexedTree (T *data, size_t size) : 
	data(data),
    size(size)
	{
        tree = new T[size]();
        tree[0] = IsZero(data[0]);
        for (int i = 1; i < size; ++i) {
            tree[i] = tree[i-1] + IsZero(data[i]);
        }
        int j = 0;
        for (int i = size - 1; i > 0; --i) {
            j = AndFunc(i)-1;
            if (j >= 0)
                tree[i] -= tree[j];
        }
 
    }
    ~BinaryIndexedTree ()
    {
        delete[] tree;
    }
 
    void Add (int index, T val)
    {
        for (int i = index; i < size; i = OrFunc(i))
            tree[i] += val;
    }
 
	void Update (int index, T val)
	{
        T old_val = data[index];
        data[index] = val;
 
        if (IsZero(val) == IsZero(old_val)) return;
 
        int diff = (IsZero(val) == 1) ? 1 : -1;
 
        for (int i = index; i < size; i = OrFunc(i)) {
            tree[i] += diff;
        }
	}
 
    int Count (int left, int right) { return Count(right) - Count(left-1); }
 
 
    int FindKthZero (int k, int left, int right)
    {
        int mid = 0, count = 0;
 
        if (k > Count(left, right)) 
            return -2;        
        
        while (left != right) {
            mid = (left + right)/2;
            count = Count(left, mid);
 
            if (k <= count) {
                right = mid;
            } else {
                left = mid + 1;
                k -= count;
            }
        }
        return left;
    }
 
private:
 
	T *data = nullptr, *tree = nullptr;
	size_t size = 0;
 
    int IsZero (int par) { return (par == 0) ? 1 : 0; }
 
	int AndFunc (int x) { return x & (x + 1); }
    int OrFunc (int x) { return x | (x + 1); }
 
    int Count (int index) {
        int ret = 0;
 
        for (int i = index; i >= 0; i = AndFunc(i)-1)
            ret += tree[i];
 
        return ret;
    }
	
};
 
int main()
{
    std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

    int n = 0;
    std::cin >> n;
    int *data = new int[n]();
 
 
    for (int i = 0; i < n; ++i) {
        std::cin >> data[i];
    }
 
    BinaryIndexedTree<int> tree = BinaryIndexedTree<int>(data, n);
 

    std::cin >> n;
    char command = 0;
    int index = 0, val = 0;
    int left = 0, right = 0, k = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> command;

        if (command == 'u') {

            std::cin >> index >> val;
            tree.Update(index - 1, val);

        } else {

            std::cin >> left >> right >> k;
            std::cout << tree.FindKthZero(k, left-1, right-1) + 1 << " "; 

        }
    }
    
    delete[] data;
 
    return 0;
}