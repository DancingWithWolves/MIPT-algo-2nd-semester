#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct DNA {
    DNA() { }
    DNA(std::string& str)
    {
        code = str;
    }
    std::string code;
    bool is_decoded = false;

    long long vertex = 0, pos = 0, index = 0;
};

class MyTree {
public:
    struct Vertex {
        Vertex() { }
        std::unordered_map<char, long long> son; //куда можем непосредственно перейти

        std::unordered_set<long long> stop_here;
        bool isLeaf = false;
    };

    MyTree()
    {
        tree.push_back(Vertex());
    }

    std::vector<long long> addWord(std::string& word, std::vector<DNA>& dnas)
    {
        if (tree.empty()) {
            tree.push_back(Vertex());
        }

        std::vector<long long> need_to_handle;

        long long vertex_pos = 0;
        for (long long i = 0; i < word.size(); i++) {
            auto a = tree[vertex_pos].son.find(word[i]);
            if (a != tree[vertex_pos].son.end()) {
                vertex_pos = a->second;
            } else {

                for (auto& i : tree[vertex_pos].stop_here) {
                    if (dnas[i].code[dnas[i].pos] == word[i]) {
                        need_to_handle.push_back(i);
                    }
                }

                tree.push_back(Vertex());
                tree[vertex_pos].son.insert({ word[i], tree.size() - 1 });
                vertex_pos = tree.size() - 1;
            }
        }
        tree[vertex_pos].isLeaf = true;
        return need_to_handle;
    }

    void AddToRoot(DNA& dna)
    {
        tree[0].stop_here.insert(dna.index);
    }

    bool Try(DNA& dna)
    {
        long long old_node = dna.index;
        long long cur_node = old_node;

        while (dna.pos < dna.code.length()) {
            std::cout << dna.pos << "  " << dna.index << " \n";
            char cur_char = dna.code[dna.pos];

            if (tree[cur_node].son.count(cur_char) > 0) {
                ++dna.pos;
                cur_node = tree[cur_node].son.find(cur_char)->second;
            } else if (tree[cur_node].isLeaf) {
                cur_node = 0;
            } else {
                break;
            }
        }

        tree[old_node].stop_here.erase(dna.index);

        if (dna.pos == dna.code.length() && tree[cur_node].son.empty()) {
            return true;
        }

        dna.vertex = cur_node;

        if (dna.pos < dna.code.length()) {
            tree[cur_node].stop_here.insert(dna.index);
        }

        return false;
    }

private:
    std::vector<Vertex> tree;
};

void PrintDnas(std::vector<DNA> dnas)
{

    printf("-----\n");
    for (auto& i : dnas) {
        printf("[%d]'s code = %s, pos = %d, vertex = %d\n", i.index, i.code, i.pos, i.vertex);
    }

    printf("-----\n");
}

std::string Shuffle(std::string& source, long long offset)
{
    std::string ret = source;

    long long size = source.size();
    for (long long i = 0; i < size; ++i) {
        ret[i] = source[(offset + i) % size];
    }
    return ret;
}

int main()
{
    long long N = 0;
    std::cin >> N;
    char cmd = 0;
    std::string str;
    MyTree tree;

    std::vector<DNA> DNAs;

    long long prev_ans = 0;

    for (long long i = 0; i < N; ++i) {

        std::cin >> cmd >> str;

        str = Shuffle(str, prev_ans);

        PrintDnas(DNAs);

        if (cmd == '?') {
            DNA dna(str);
            dna.index = DNAs.size();
            DNAs.push_back(dna);
            tree.AddToRoot(dna);

            if (!tree.Try(DNAs.back())) {
                prev_ans = 0;
                std::cout << "0";
            } else {
                DNAs.back().is_decoded = true;
                prev_ans = 1;
                std::cout << "1 " << DNAs.back().index + 1;
            }
            std::cout << std::endl;

        } else if (cmd == '+') {

            std::vector<long long> ans;

            std::vector<long long> checkable = tree.addWord(str, DNAs);

            for (auto& index : checkable) {
                if (tree.Try(DNAs[index])) {
                    std::cout << index << " 1111\n";
                    ans.push_back(index);
                }
            }

            std::cout << ans.size() << " ";
            prev_ans = ans.size();
            for (long long i = 0; i < ans.size(); ++i) {
                std::cout << ans[i] + 1 << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
