#include <cstdlib>
#include <iostream>




template <typename T> 
class BinaryIndexedTree {
private:
	T* tree = nullptr;
	T* array = nullptr;
	const size_t size = 0;

public:
	BinaryIndexedTree(T* array, size_t n): array(array), size(n){

		tree = new T[n]();
		tree[0] = is_zero(array[0]);
		for (int i = 1; i < n; ++i) {
			tree[i] = tree[i - 1] + is_zero(array[i]);
		}

		int j = 0;
		for (int i = n - 1; i > 0; --i){
			j = (i & (i + 1)) - 1;
			if (j >= 0){
				tree[i] -= tree[j];
			}
		}
	}

	void add(int elem, T val){

		for (int i = elem; i < size; i = i | (i + 1)){
			tree[i] += val;
		}
	}

	int count_zeroes(int elem){

		int s = 0;

		for (int i = elem; i >= 0; i = (i & (i + 1)) - 1){
			s += tree[i]; 
		}

		return s;
	}

	T count_zeroes(int l, int r){
		return count_zeroes(r) - count_zeroes(l - 1);
	}

	void update(int elem_i, int new_val){

		int old_val = array[elem_i];
		array[elem_i] = new_val;

		if (is_zero(new_val) == is_zero(old_val)) return;

		int d = (is_zero(new_val)?(1):(-1));

		for (int i = elem_i; i < size; i = i | (i + 1)){
			tree[i] += d;
		}
	}


	int find_kth_zero(int k, int l_bound, int r_bound){

		int start = l_bound;
		int middle = 0;
		int count = 0;

		if (k > count_zeroes(l_bound, r_bound)){
			return -2;
		}
		
		while (l_bound != r_bound){

			middle = (r_bound + l_bound) >> 1;
			count = count_zeroes(l_bound, middle);

			if (k <= count){
				r_bound = middle;
			} 
			else {
				l_bound = middle + 1;
				k -= count;
			}
		}

		return (l_bound);
	}
    int is_zero(int num){ return (num == 0) ? 1 : 0; }
};

int main(void){

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	
	int n = 0;
	std::cin >> n;

	int* data = new int[n]();
	
	for (int i = 0; i < n; ++i){
		std::cin >> data[i];
	};

	auto t = BinaryIndexedTree<int>(data, n);

	int m = 0;
	std::cin >> m;

	char c;
	int elem_i, new_val;
	int l_bound, r_bound, k;

	for (int i = 0; i < m; ++i){
		std::cin >> c;

		if (c == 'u'){
			std::cin >> elem_i;
			std::cin >> new_val;

			t.update(elem_i - 1, new_val);
	    }

		else if (c == 's'){
			std::cin >> l_bound;
			std::cin >> r_bound;
			std::cin >> k;

			std::cout << t.find_kth_zero(k, l_bound - 1, r_bound - 1) + 1 << " ";
		}
	}

	return 0;
}