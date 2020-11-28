#include <iostream>
#include<unordered_map>
#include<string>
#include<vector>


const char letters[] = {'0', '1'};

class MyTree {

    typedef enum {initial, this_dfs, not_this_dfs} Status;

public:
    struct Vertex {
        Status status = initial;
        std::unordered_map<char, int> son; //куда можем непосредственно перейти
        std::unordered_map<char, int> goTo; //куда переходим по конкретной букве(son + суффиксные ссылки)
        std::pair<int, char> parent; //родитель + по какой букве от него пришли
        std::vector<int> endOfWord;
        int wordSize = 0;
        int suffixLink = -1;
        int suffixLinkUp = -1;
        bool isLeaf = false;
    };

    MyTree() {
        tree.push_back(Vertex());
    }

    void addWord(std::string& word, int& numOfWord) {
        if (tree.empty()) {
            tree.push_back(Vertex());
        }
        int vertex_pos = 0;
        for (int i = 0; i < word.size(); i++) {
            auto a = tree[vertex_pos].son.find(word[i]);
            if (a != tree[vertex_pos].son.end()) {
                vertex_pos = a->second;
            } else {
                tree.push_back(Vertex());
                tree.back().parent.first = vertex_pos;
                tree.back().parent.second = word[i];
                tree[vertex_pos].son.insert({word[i], tree.size() - 1});
                vertex_pos = tree.size() - 1;
            }
        }
        tree[vertex_pos].endOfWord.push_back(numOfWord);
        tree[vertex_pos].isLeaf = true;
        tree[vertex_pos].wordSize = word.size();
    }

    int TermLink(int node) {
        if (tree[node].isLeaf) {
            return node;
        } else {
            return tree[node].suffixLinkUp;
        }
    }

    bool process_rec(int vertex)
    {
        if (tree[vertex].status == this_dfs) {
            return true;
        } else if (tree[vertex].status == not_this_dfs) {
            return false;
        }
        tree[vertex].status = this_dfs;

        for (int i = 0; i < 2; ++i) {
            int next = Jump(vertex, letters[i]);
            if (TermLink(next) == 0 && process_rec(next)) {
                return true;
            }
        }
        tree[vertex].status = not_this_dfs;
        return false;
    }

    bool process() {
        makeSuffixLinksUp();

        return process_rec(0);
    }

    void makeSuffixLinksUp() {
        for (int i = 0; i < tree.size(); i++) {
            getSuffixLinkUp(i);
        }
    } 

    int getSuffixLink(int vert) {
        if (tree[vert].suffixLink == -1) {
            if (vert == 0 || tree[vert].parent.first == 0) {
                tree[vert].suffixLink = 0;
            } else {
                tree[vert].suffixLink = Jump(getSuffixLink(tree[vert].parent.first), tree[vert].parent.second);
            }
        }
        return tree[vert].suffixLink;
    }

    int Jump(int vert, char letter) {
        auto a = tree[vert].goTo.find(letter);
        if (a == tree[vert].goTo.end()) {
            auto b = tree[vert].son.find(letter);
            if (b != tree[vert].son.end()) {
                tree[vert].goTo.insert({letter, b -> second});
            } else if (vert == 0) {
                tree[vert].goTo.insert({letter, 0});
            } else {
                tree[vert].goTo.insert({letter, Jump(getSuffixLink(vert), letter)});
            }
        }
        return tree[vert].goTo.find(letter) -> second;
    }

    int getSuffixLinkUp (int vert) {
        if (tree[vert].suffixLinkUp == -1) {
            int suf = getSuffixLink(vert);
            if (tree[suf].isLeaf) {
                tree[vert].suffixLinkUp = suf;
            } else if (suf == 0) {
                tree[vert].suffixLinkUp = 0;
            } else {
                tree[vert].suffixLinkUp = getSuffixLinkUp(suf);
            }
        }
        return tree[vert].suffixLinkUp;
    }

private:

    std::vector<Vertex> tree;
}; 


int main() {
    std::string word;
    MyTree tree;
    

    int N = 0;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> word;
        tree.addWord(word, i);
    }

    if (tree.process()) {
        std::cout << "TAK" << std::endl;
    } else {
        std::cout << "NIE" << std::endl;
    }
    
    
    
    return 0;
}
