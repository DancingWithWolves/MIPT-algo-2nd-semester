
#include <vector>
#include <iostream>
 
 
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

 
 
int main() 
{
    int A = 0, B = 0;

    std::cin >> A >> B;
    
    std::vector<std::vector<int>> neighbours (A);

    std::vector<int> ans;
 
    for (int i = 0; i < A; ++i) {

        int neighbour = 0;
        std::cin >> neighbour;
        while (neighbour) {
            neighbours[i].push_back(neighbour - 1);
            std::cin >> neighbour;
        }
    }


    int edges_qty = MaxMatch(neighbours, ans);

    std::cout << edges_qty << std::endl;

    for (int i = 0; i < ans.size(); ++i) {
        if (ans[i] != -1)
            std::cout << ans[i] + 1 << ' ' << i + 1 << std::endl;
    }
}