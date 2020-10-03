#include <fstream>
#include <vector>

long long n = 0, m = 0, a = 0, b = 0;
long long free_cells = 0;
std::vector<std::vector<int>> board;

std::ifstream in;
std::ofstream out;

std::vector<std::vector<long long>> GraphFromBoard() 
{
    std::vector<std::vector<long long>> res (n * m);
 
    for (long long cur_row = 0; cur_row < n; ++cur_row) {

        for (long long cur_column = 0; cur_column < m; ++cur_column) {

            // В одной доле -- клетки только одного цвета (более того, только с такими графами работает алгоритм из задаки А)
            if ( (cur_row + cur_column) % 2 == 1 )
                continue;
 
            if ( board[cur_row][cur_column] == 0 ) {
                // левый сосед
                if (cur_column >= 1 && board[cur_row][cur_column - 1] == 0) {
                    res[m * cur_row + cur_column].push_back(m * cur_row + (cur_column - 1));
                }
                // правый сосед
                if (cur_row + 1 < n && board[cur_row + 1][cur_column] == 0) {
                    res[m * cur_row + cur_column].push_back(m * (cur_row + 1) + cur_column);
                }
                // нижний сосед
                if (cur_column + 1 < m && board[cur_row][cur_column + 1] == 0) {
                    res[m * cur_row + cur_column].push_back(m * cur_row + (cur_column + 1));
                }
                // верхний сосед
                if (cur_row >= 1 && board[cur_row - 1][cur_column] == 0) {
                    res[m * cur_row + cur_column].push_back(m * (cur_row - 1) + cur_column);
                }
            }
        }
    }
 
    return res;
}
 
 

bool FindChain (  long long cur_node
                , std::vector<std::vector<long long>>& neighbours
                , std::vector<bool>& saturated
                , long long& match_size
                , std::vector<long long>& ans) 
{
    if (saturated[cur_node])
        return false;
 
    saturated[cur_node] = true;

    for (auto next : neighbours[cur_node]) {

        if (next >= ans.size())
            ans.resize(next + 1, -1);
 
        if (ans[next] == -1) {

            ans[next] = cur_node;
            ++match_size;
            return true;
 
        } else {

            if (FindChain(ans[next], neighbours, saturated, match_size, ans)) {
                ans[next] = cur_node;
                return true;
            }
        }
    }
 
    return false;
}
 
 
long long MaxMatch(std::vector<std::vector<long long>> neighbours, std::vector<long long>& ans) 
{
    long long match_size = 0;

    std::vector<bool> saturated;

    for (long long i = 0; i < neighbours.size(); ++i) {
        saturated.assign(neighbours.size(), false);
        FindChain(i, neighbours, saturated, match_size, ans);
    }
 
    return match_size;
}
 
 
long long Count() {

    if (a >= 2 * b) {
        return b * free_cells;
    }
 
    std::vector<std::vector<long long>> graph = GraphFromBoard();

    std::vector<long long> found;
    long long double_qty = MaxMatch(graph, found);
 
    return double_qty * a + (free_cells - 2 * double_qty) * b;
}
 
 
int main() 
{
    in.open("dominoes.in");
    out.open("dominoes.out");


    in >> n >> m >> a >> b;
 
    board.assign(n, std::vector<int>(m));

    for (long long cur_row = 0; cur_row < n; ++cur_row) {
        
        for (long long cur_column = 0; cur_column < m; ++cur_column) {
            
            char cur = 0;
            in >> cur;

            switch (cur) {

                case '*':
                    board[cur_row][cur_column] = 0;
                    ++free_cells;
                    break;
    
                case '.':
                    board[cur_row][cur_column] = 1;
                    break;
            
            }
        }
    }

    long long ans = Count();
    out << ans << std::endl;
}