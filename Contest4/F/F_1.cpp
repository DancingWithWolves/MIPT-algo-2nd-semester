#include <stdio.h>
#include <vector>


struct triple_t{
	int first  = 0;
	int second = 0;
	int third  = 0;
};


class SegmentTree {
public:

	std::vector<triple_t> tree;
	size_t size;

	SegmentTree (size_t size): size(size)
    {
		tree = std::vector<triple_t>(4*size);
	}

	void UpdateIter (size_t i, size_t query_left, size_t query_right, size_t left, size_t right, int diff) 
    {

		if (query_right < left || query_left > right)
			return;

		if (query_left <= left && right <= query_right) {
			tree[i].first += diff;
			tree[i].second += diff;
			tree[i].third += diff;
			return;
		}

		size_t mid = (left + right) / 2;

		UpdateIter(2*i, query_left, query_right, left, mid, diff);
		UpdateIter(2*i + 1, query_left, query_right, mid + 1, right, diff);

		tree[i].first = min(tree[2*i].first, tree[2*i + 1].first) + tree[i].third;
		tree[i].second = max(tree[2*i].second, tree[2*i + 1].second) + tree[i].third;
	}

	void Update(size_t l, int diff)
    {
		UpdateIter(1, l, size, 1, size, diff);
	}

    private:
    int max (int a, int b) { return (a>b)?(a):(b); }

    int min (int a, int b) { return (b>a)?(a):(b); }
};



int main(void){

	int n = 0;
    scanf("%d", &n);

	int num = 0, side = 0;

	auto seg_tree = SegmentTree(n);

	for (int i = 0; i < n; ++i) {
        scanf("%d%d", &num, &side);

		if (side == 1) seg_tree.Update(n + 1 - num,  1);
		if (side == 2) seg_tree.Update(n + 1 - num, -1);

		if (seg_tree.tree[1].first >= 0) printf(">\n");
		else if (seg_tree.tree[1].second <= 0) printf("<\n");
		else printf("?\n");
	}

	return 0;
}