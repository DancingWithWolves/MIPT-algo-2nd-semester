#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ostream>
#include <vector>

template <class T, T(*Max)(T, T)> 
class SegmentTree {

public:

    void BuildTree (int i, int left, int right)
    {
        if (left == right) {
            tree_data[i] = data[left];
            return;
        }

        int mid = (left + right) /2;
        BuildTree(2*i, left, mid);
        BuildTree(2*i+1, mid+1, right);

        tree_data[i] = Max(tree_data[2*i], tree_data[2*i+1]);
    }
    SegmentTree (std::vector<T> data) : data(data)
    {
        tree_data = std::vector<T>(4*data.size());
        BuildTree(1, 0, data.size()-1);
    }

    T GetMaxIter (int i, int query_left, int query_right, int vertex_left, int vertex_right)
    {
        if (query_left <= vertex_left && query_right >= vertex_right)
            return tree_data[i];
        if (query_right < vertex_left || query_left > vertex_right)
            return std::pair<int, int>(0, 0);

        int mid = (vertex_left + vertex_right)/2;
        return Max(GetMaxIter(2*i, query_left, query_right, vertex_left, mid), 
                   GetMaxIter(2*i+1, query_left, query_right, mid + 1, vertex_right ));
    }

    T GetMax (int left, int right)
    {
        return GetMaxIter (1, left, right, 0, data.size()-1);
    }

private:
    std::vector<T> data;
    std::vector<T> tree_data;
};

std::pair<int, int> Max (std::pair<int, int> first, std::pair<int, int> second )
{
    return (first.first > second.first) ? first : second;
}

int main ()
{
    int n = 0, cur = 0;

    FILE *in = fopen("index-max.in", "r");
    FILE *out = fopen("index-max.out", "w");

    fscanf(in, "%d", &n);
    std:std::vector<std::pair<int, int>> data(n);    
    
    for (int i = 0; i < n; ++i) {
        fscanf(in, "%d", &cur);
        data[i] = std::pair<int, int>(cur, i);
    }
    
    SegmentTree<std::pair<int, int>, Max> seg_tree (data);

    int query_left = 0, query_right = 0;
    fscanf(in, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(in, "%d%d", &query_left, &query_right);
        fprintf(out, "%d\n", seg_tree.GetMax(query_left-1, query_right-1).second + 1);
    }
    fclose (in);
    fclose (out);
    return 0;
}