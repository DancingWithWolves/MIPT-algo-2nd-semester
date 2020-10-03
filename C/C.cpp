#include <iostream>
#include <vector>
#include <math.h>

const int end_of_the_day = 23 * 60 + 59;
int M = 0;

struct Order {
    int time_begin = 0;
    int time_end = 0;
    int from_x = 0;
    int from_y = 0;
    int to_x = 0;
    int to_y = 0;
};

 
bool FindChain (  int curNode
                , std::vector<std::vector<int>>& neighbours
                , std::vector<bool>& saturated
                , int& match_size
                , std::vector<int>& ans) 
{
    if (saturated[curNode])
        return false;
 
    saturated[curNode] = true;

    for (auto next : neighbours[curNode]) {

        if (next >= ans.size())
            ans.resize(next + 1, -1);
 
        if (ans[next] == -1) {

            ans[next] = curNode;
            ++match_size;
            return true;
 
        } else {

            if (FindChain(ans[next], neighbours, saturated, match_size, ans)) {
                ans[next] = curNode;
                return true;
            }
        }
    }
 
    return false;
}
 
 
int MaxMatch(std::vector<std::vector<int>> neighbours, std::vector<int>& ans) 
{
    int match_size = 0;

    std::vector<bool> saturated;

    for (int i = 0; i < neighbours.size(); ++i) {
        saturated.assign(neighbours.size(), false);
        FindChain(i, neighbours, saturated, match_size, ans);
    }

    return match_size;
}


void CreateGraph(std::vector<Order> &orders, std::vector<std::vector<int>> &neighbours)
{
    for (int i = 0; i < M; ++i) {

        for (int j = 0; j < M; ++j) {
            if (i == j) {
                continue;
            }
            if (orders[i].time_end + fabs(orders[i].to_x - orders[j].from_x) + fabs (orders[i].to_y - orders[j].from_y) < orders[j].time_begin) {
                neighbours[i].push_back(j);
            }

        }
    }

}

int main ()
{
    std::cin >> M;
    std::vector<Order> orders (M);

    for (int i = 0; i < M; ++i) {
        char trash = 0;
        int time_h = 0, time_m = 0;

        std::cin >> time_h >> trash >> time_m >> orders[i].from_x >> orders[i].from_y >> orders[i].to_x >> orders[i].to_y;

        orders[i].time_begin = 60 * time_h + time_m;
        orders[i].time_end = orders[i].time_begin + fabs(orders[i].from_x - orders[i].to_x) + fabs(orders[i].from_y - orders[i].to_y);

    }

    std::vector<std::vector<int>> neighbours (M);
    CreateGraph(orders, neighbours);

    std::vector<int> ans;
    int match_size = MaxMatch(neighbours, ans);

    std::cout << M - match_size << std::endl;
    return 0;
}