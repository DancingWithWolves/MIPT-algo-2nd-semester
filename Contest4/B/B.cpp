#include <iostream>
#include <vector>

using namespace std;


template <class T>

class BinaryIndexedTree3d {
public:

	BinaryIndexedTree3d (int edge_len) : 
	edge_len(edge_len), 
	data (vector<vector<vector<T>>>(edge_len, vector<vector<T>>(edge_len, vector<T>(edge_len, {}))))
	{}

	int AreaSum(int x1, int y1, int z1, int x2, int y2, int z2) 
	{
		if (x1 > x2)
			swap(x1, x2);
		
		if (y1 > y2)
			swap(y1, y2);
		
		if (z1 > z2)
			swap(z1, z2);
		

		int bigCube = Prefix(x2, y2, z2);
		int smallCube = Prefix(x1 - 1, y1 - 1, z1 - 1);
		int subVal = Prefix(x1 - 1, y2, z2) + Prefix(x2, y1 - 1, z2) + Prefix(x2, y2, z1 - 1);
		int addVal = Prefix(x2, y1 - 1, z1 - 1) + Prefix(x1 - 1, y2, z1 - 1) + Prefix(x1 - 1, y1 - 1, z2);

		return bigCube - subVal + addVal - smallCube;
	}

	void Update(int x, int y, int z, T diff)
	{

		for (int i = x; i < edge_len; i = OrFunc(i)) {
			for (int j = y; j < edge_len; j = OrFunc(j)) {
				for (int k = z; k < edge_len; k = OrFunc(k)) {
					data[i][j][k] += diff;
				}
			}
		}
	}

private:

	vector<vector<vector<T>>> data;
	int edge_len = 0;

	int AndFunc(int x) { return x & (x + 1); }

	int OrFunc(int x) {	return x | (x + 1);	}

	int Prefix(int x, int y, int z) 
	{
		int res = 0;

		for (int i = x; i >= 0; i = AndFunc(i) - 1) {
			for (int j = y; j >= 0; j = AndFunc(j) - 1) {
				for (int k = z; k >= 0; k = AndFunc(k) - 1) {
					res += data[i][j][k];
				}
			}
		}

		return res;
	}
};

enum command_type {
	update_command = 1,
	count_command = 2,
	end_command = 3
};


int main() 
{
	unsigned short n = 0;
	scanf("%hu", &n);

	BinaryIndexedTree3d<int> stars_tree(n);

	unsigned short cur_command = 0;
	scanf("%hu", &cur_command);
	while (cur_command != end_command) {
		switch (cur_command) {

			case update_command: {
				int x = 0, y = 0, z = 0, diff = 0;
				scanf("%d%d%d%d", &x, &y, &z, &diff);
				stars_tree.Update(x, y, z, diff);
				break;
			}
			case count_command: {
				int x1 = 0, y1 = 0, z1 = 0, x2 = 0, y2 = 0, z2 = 0;
				scanf("%d%d%d%d%d%d", &x1, &y1, &z1, &x2, &y2, &z2);
				printf( "%d\n", stars_tree.AreaSum(x1, y1, z1, x2, y2, z2) );
				break;
			}
			default:
				break;	
		}

		scanf("%hu", &cur_command);
	}

	return 0;
}