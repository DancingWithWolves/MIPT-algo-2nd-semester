#include <iostream>
#include<unordered_map>
#include<string>
#include<vector>

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

class MyTree {
public:

    MyTree() {
        tree.push_back(Vertex());
    }

    void addWord(std::string& word, int& numOfWord) {
        if (tree.empty()) {
            tree.push_back(Vertex());
        }
        int vertexPos = 0;
        for (int i = 0; i < word.size(); i++) {
            auto a = tree[vertexPos].son.find(word[i]);
            if (a != tree[vertexPos].son.end()) {
                vertexPos = a->second;
            } else {
                tree.push_back(Vertex());
                tree.back().parent.first = vertexPos;
                tree.back().parent.second = word[i];
                tree[vertexPos].son.insert({word[i], tree.size() - 1});
                vertexPos = tree.size() - 1;
            }
        }
        tree[vertexPos].endOfWord.push_back(numOfWord);
        tree[vertexPos].isLeaf = true;
        tree[vertexPos].wordSize = word.size();
    }

    void process(std::string source, std::vector<int>& delta, std::vector<int>& countWords) {
        int vertexPos = 0;
        int goUp = 0;
        makeSuffixLinksUp();
        for (int i = 0; i < source.size(); ++i) {
            vertexPos = trans(vertexPos, source[i]);
            goUp = vertexPos;
            while (goUp != 0) {
                for (int k = 0; k < tree[goUp].endOfWord.size(); k++) {
                    int end = i - tree[goUp].wordSize + 1 + delta[tree[goUp].endOfWord[k]];
                    if (end < countWords.size()) { //проверка, что не вышли за пределы
                        ++countWords[end];
                    }
                }
                goUp = tree[goUp].suffixLinkUp;
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
                tree[vert].suffixLink = trans(getSuffixLink(tree[vert].parent.first), tree[vert].parent.second);
            }
        }
        return tree[vert].suffixLink;
    }

    int trans(int vert, char letter) {
        auto a = tree[vert].goTo.find(letter);
        if (a == tree[vert].goTo.end()) {
            auto b = tree[vert].son.find(letter);
            if (b != tree[vert].son.end()) {
                tree[vert].goTo.insert({letter, b -> second});
            } else if (vert == 0) {
                tree[vert].goTo.insert({letter, 0});
            } else {
                tree[vert].goTo.insert({letter, trans(getSuffixLink(vert), letter)});
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
    std::string pattern;
    std::string source;
    std::cin >> pattern >> source;
    MyTree tree;
    std::string word;
    int numOfWord = 0;
    std::vector<int>delta; //смещение начал слов
    std::vector<int> countWords(source.size(), 0);
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != '?') {
            word += pattern[i];
        } else {
            if (word.size()) {
                tree.addWord(word, numOfWord);
                ++numOfWord;
                delta.push_back(pattern.size() - i + word.size() - 1);
                word = "";
            }
        }
    }
    if (word.size()) {
        tree.addWord(word, numOfWord);
        ++numOfWord;
        delta.push_back(pattern.size() - pattern.size() + word.size() - 1);
        word = "";
    }
    tree.process(source, delta, countWords);
    for (int i = 0; i < source.size(); i++)
    {
        if(countWords[i] == numOfWord && i + 1 - (pattern.size()) >= 0){
            std::cout << (i + 1 - pattern.size()) << " ";
        }
    }
    
    return 0;
}