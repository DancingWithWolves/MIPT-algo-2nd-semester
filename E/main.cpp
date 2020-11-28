#include <iostream>
#include<unordered_map>
#include<string>
#include<vector>



class MyTree {
public:
    struct Vertex {
        Vertex() {}
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

    void process(std::string source, std::vector<std::vector<int>>& count_words) {

        int vertex_pos = 0;
        int go_up = 0;

        makeSuffixLinksUp();

        for (int i = 0; i < source.size(); ++i) {
            vertex_pos = Jump(vertex_pos, source[i]);
            go_up = vertex_pos;

            while (go_up != 0) {
                for (int k = 0; k < tree[go_up].endOfWord.size(); k++) {
                    count_words[tree[go_up].endOfWord[k]].push_back(i - tree[go_up].wordSize + 1);
                }
                go_up = tree[go_up].suffixLinkUp;
            }
        }
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
    std::string source, word;
    std::cin >> source;
    MyTree tree;
    

    int N = 0;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> word;
        tree.addWord(word, i);
    }

    std::vector<std::vector<int>> count_words(N);
    tree.process(source, count_words);
    for (int i = 0; i < N; ++i) {
        std::cout << count_words[i].size() << " ";

        for (int j = 0; j < count_words[i].size(); ++j) {
            std::cout << count_words[i][j] + 1 << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}