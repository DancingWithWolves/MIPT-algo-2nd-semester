#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
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
        bool isLeaf = false;
    };

    MyTree()
    {
        tree.push_back(Vertex());
    }

    void addWord(std::string& word)
    {
        if (tree.empty()) {
            tree.push_back(Vertex());
        }
        long long vertex_pos = 0;
        for (long long i = 0; i < word.size(); i++) {
            auto a = tree[vertex_pos].son.find(word[i]);
            if (a != tree[vertex_pos].son.end()) {
                vertex_pos = a->second;
            } else {
                tree.push_back(Vertex());
                tree[vertex_pos].son.insert({ word[i], tree.size() - 1 });
                vertex_pos = tree.size() - 1;
            }
        }
        // tree[vertex_pos].endOfWord.push_back(numOfWord);
        tree[vertex_pos].isLeaf = true;
    }

    bool Try(DNA &dna)
    {
        int cur = dna.vertex;
        int old = cur;
        while (dna.pos < dna.code.length()) {
            char cur_char = dna.code[dna.pos];

            if (tree[cur].son.count(cur_char) > 0) {
                dna.pos++;
                cur = tree[cur].son.find(cur_char)->second;
            } else if (tree[cur].isLeaf) {
                cur = 0;
            } else {
                break;
            }
        }

        if (dna.pos == dna.code.length() && tree[cur].isLeaf) {
            return true;
        }
        
    }

private:
    std::vector<Vertex> tree;
};

void PrintDnas(std::vector<DNA> dnas)
{

    printf("-----\n");
    for (auto &i : dnas) {
        printf("[%d]'s code = %s, pos = %d, vertex = %d\n", i.index, i.code, i.pos, i.vertex);
    }

    printf("-----\n");
}

std::string Shuffle(std::string &source, long long offset)
{
    std::string ret = source;

    long long size = source.length();
    for (long long i = 0; i < size; ++i) {
        ret[i] = source[(offset+i)%size];
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

        // PrintDnas(DNAs);

        if (cmd == '?') {
            DNA dna(str);
            dna.index = DNAs.size();
            DNAs.push_back(dna);
            

            if (!tree.Try(DNAs.back())) {
                prev_ans = 0;
                std::cout << "0";
            } else {
                DNAs.back().is_decoded = true;
                prev_ans = 1;
                std::cout << "1 " << DNAs.back().index + 1;
            }

        } else if (cmd == '+') {

            std::vector<long long> ans;

            tree.addWord(str);
            
            for (auto& i : DNAs) {
                if (!i.is_decoded) {
                    if (tree.Try(i)) {
                        ans.push_back(i.index);
                        i.is_decoded = true;
                    }
                }
            }

            std::cout << ans.size() << " ";
            prev_ans = ans.size();
            for (long long i = 0; i < ans.size(); ++i) {
                std::cout << ans[i] + 1 << " ";
            }
        }

        std::cout << std::endl;
    }

    return 0;
}
