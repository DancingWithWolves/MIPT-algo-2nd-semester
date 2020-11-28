///// A


#include <iostream>
#include <vector>
#include <exception>
#include <cassert>
#include <algorithm>
#include <set>

template<typename TypeName>
class PolHash {
    uint64_t P;
    uint64_t M;

    std::vector<uint64_t> pow_P;
    std::vector<uint64_t> hash;

public:
    PolHash (const std::vector<TypeName>& str, uint64_t P = 433, uint64_t M = 1073676287);

    uint64_t hash_from_l_to_r (std::size_t l, std::size_t r) {
        int max_size = pow_P.size ();
        assert (l >= 0);
        assert (l < max_size);
        assert (r>= 0);
        assert (r <= max_size);

        return hash[r - 1] - (l == 0 ? 0 : hash[l - 1] * pow_P[r - l]);
    }
};

class TaskB {

    uint64_t N;
    uint64_t M;
    std::vector<uint64_t> colors;
    std::set<uint64_t> ans;

public:
    TaskB () = default;
    void solve ();


    friend std::istream& operator>> (std::istream &in, TaskB &task);
    friend std::ostream& operator<< (std::ostream &out, const TaskB &task);
};


int main (const int argc, const char** argv)
{
    TaskB task;
    std::cin >> task;
    task.solve ();
    std::cout << task;
}



std::istream &operator>> (std::istream &in, TaskB &task)
{
    in >> task.N >> task.M;
    task.colors.resize (task.N);
    for (auto& color : task.colors) {
        in >> color;
    }
    return in;
}

std::ostream &operator<< (std::ostream &out, const TaskB &task)
{
    for (const auto& i : task.ans) {
        std::cout << i << " ";
    }
    return out;
}



void TaskB::solve ()
{
    PolHash<uint64_t> hash_normal (colors);
    auto reserve_colors = colors;
    std::reverse (reserve_colors.begin (), reserve_colors.end ());
    PolHash<uint64_t> hash_reverse (reserve_colors);

    for (std::size_t i = (N + 1)/2 - 1; i < N - 1; i++) {

        uint64_t hash1 = hash_normal.hash_from_l_to_r (0, N - i);
        uint64_t hash2 = hash_reverse.hash_from_l_to_r (2 * i - N + 2, i + 2);

        if (hash1 == hash2) {
            if (colors[colors.size () - 2 - i] == colors[colors.size () - 1 - i])
            {
                for (int j = 0; j < i + 1; ++j)
                {
                    if (colors[colors.size () - 2 - i - j] != colors[colors.size () - 1 - i + j])
                    {
                        break;
                    }


                    if (colors.size () - 2 - i - j == 0)
                    {
                        ans.insert (i + 1);
                        break;
                    }
                }
            }
        }
    }
    ans.insert (N);
}

template<typename TypeName>
PolHash<TypeName>::PolHash (const std::vector<TypeName>& str, uint64_t P, uint64_t M):
        P (P),
        M (M)
{
    int str_size = str.size ();

    pow_P.resize (str_size);
    hash.resize  (str_size);

    pow_P[0] = 1;
    hash[0]  = str[0];
    for (std::size_t i = 1; i < str_size; i++) {
        pow_P[i] = (pow_P[i - 1] * P);
        hash[i] = (hash[i - 1] * P + str[i]);
    }
}


///// C

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>



using namespace std;

class Trie {
public:

    struct node {
        unordered_map<char, int> go;
        bool terminal;
        unordered_map<char, vector<pair<int, int>>> kids;
        node() {
            terminal = false;
        }
    };


    vector<node> trie;

    Trie () {
        trie.emplace_back();
    }

    size_t add_node() {
        trie.emplace_back();
        return trie.size () - 1;
    }

    int add_string(const string& str, int v, int str_number, int i) {
        for (; i < str.size ();) {
            if (trie[v].terminal) {
                v = 0;
                continue;
            }
            if (trie[v].go.find (str[i] ) == trie[v].go.end ()) {
                trie[v].kids[str[i] ].push_back({str_number, i});
                return 0;
            }
            v = trie[v].go[str[i] ];
            ++i;
        }

        if (i == str.size ()) {
            return trie[v].terminal;
        }
        trie[v].kids[str[i]].push_back({str_number, i});
        return 0;
    }
};




int main() {
    Trie trie;

    
    int n;
    cin >> n;

    vector<string> DNK;
    int  new_answers = 0;
    char operation   = 0;
    string str;
    vector<int> ans;

    for (int i = 0; i < n; i++) {

        cin >> operation >> str;
        int str_size = str.size ();
        str = str.substr (new_answers % str_size , str_size  - new_answers % str_size ) + str.substr (0, new_answers % str_size );

        if (operation == '+') {
            ans.clear ();
            int node = 0;

            for (int j = 0; j < str_size ; j++) {
                int symbol = str[j] ;

                if (trie.trie[node].go.end () != trie.trie[node].go.find (symbol)) {
                    node = trie.trie[node].go[symbol];
                    continue;
                }

                int new_node = trie.trie[node].go[symbol] = trie.add_node();
                for (auto& [numbers_of_strs, position]: trie.trie[node].kids[symbol]) {
                    if (DNK[numbers_of_strs].size () > position + 1) {
                        position++;
                        trie.trie[new_node].kids[DNK[numbers_of_strs][position]].emplace_back (numbers_of_strs, position);
                    } else {
                        if (j == str_size  - 1) {
                            ans.push_back(numbers_of_strs);
                        }
                    }
                }

                trie.trie[node].kids[symbol].clear();
                node = new_node;
            }
            trie.trie[node].terminal = true;

            for (auto& x: trie.trie[node].kids) {
                for (auto [numbers_of_strs, position]: x.second) {
                    if (trie.add_string(DNK[numbers_of_strs], 0, numbers_of_strs, position)) {
                        ans.push_back(numbers_of_strs);
                    }
                }
                x.second.clear();
            }

            new_answers = ans.size ();
            cout << ans.size () << " ";
            for (auto l : ans) {
                cout << l + 1 << " ";
            }
            cout << "\n";
        } else {

            DNK.push_back(str);
            if (trie.add_string(str, 0, DNK.size () - 1, 0)) {
                cout << 1 << " " << DNK.size () << "\n";
                new_answers = 1;
            } else {
                cout << 0 << "\n";
                new_answers = 0;
            }
        }
    }
}


////// D


#include <iostream>
#include <vector>
#include <algorithm>

class TaskD {
private:
    std::vector<std::string> dictionary;
    std::vector<int32_t>     dots;
public:
    TaskD () = default;
    void solve ();

    friend std::istream& operator>> (std::istream &in, TaskD &task);
    friend std::ostream& operator<< (std::ostream &out, const TaskD &task);
};



int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    
    TaskD task;
    std::cin >> task;
    task.solve ();
    std::cout << task;
}

std::istream &operator>> (std::istream &in, TaskD &task)
{
    std::string str;
    std::cin >> str;

    task.dots.resize(str.size());
    std::string tmp;
    std::size_t str_size = str.size ();
    for (std::size_t i = 0; i < str_size; i++) {

        if (str[i] == '.'){
            if (tmp != "") {
                task.dictionary.push_back (tmp);
            }

            task.dots[task.dictionary.size()]++;
            tmp = "";
            continue;
        } else {
            tmp.push_back (str[i]);
        }

        if (i == str_size - 1) {
            task.dictionary.push_back (tmp);
        }
    }

    return in;
}

void TaskD::solve ()
{
    std::sort (dictionary.begin (), dictionary.end ());
}

std::ostream &operator<< (std::ostream &out, const TaskD &task)
{
    std::size_t dick_size = task.dictionary.size ();
    for (std::size_t i = 0; i < dick_size; i++) {
        for (std::size_t k = 0; k < task.dots[i]; k++) {
            out << '.';
        }
        out << task.dictionary[i];
    }

    for (std::size_t k = 0; k < task.dots[dick_size]; k++) {
        out << '.';
    }
    return out;
}



////// E


#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <string>

class Trie {
private:
    struct Node {

        Node (char last_symbol = -1, const std::shared_ptr<Node>& parent = nullptr):
            is_terminal (false),
            last_symbol (last_symbol),
            suff_link   (),
            condenced_suff_link (),
            parent      (parent)
        { }

        std::unordered_map<char, std::shared_ptr<Node>> go;
        std::unordered_map<char, std::shared_ptr<Node>> kids;
        std::shared_ptr<Node> parent;

        int is_terminal;
        char last_symbol;

        std::vector<int> str_numbers;
        std::shared_ptr<Node> suff_link;
        std::shared_ptr<Node> condenced_suff_link;
    };


public:
    std::shared_ptr<Node> root;


    Trie ():
        root (new Node)
    {
        root->suff_link = root;
        root->condenced_suff_link = root;
    }

    void add_str (const std::string& str, int str_number) {
        auto vertex = root;
        std::size_t i = 0;
        std::size_t str_len = str.size ();
        for (; i != str_len && vertex->go.find (str[i]) != vertex->go.end (); i++) {
            vertex = vertex->go[str[i]];
        }

        for (; i != str_len; i++) {
            vertex->go[str[i]] = std::make_shared<Node> (str[i], vertex);
            vertex = vertex->go[str[i]];
            vertex->suff_link = root;
            vertex->condenced_suff_link = root;

        }

        vertex->str_numbers.push_back (str_number);
        vertex->is_terminal++;

    }

    std::shared_ptr<Node> get_suff_link (const std::shared_ptr<Node>& cur_node) {
        if (cur_node->suff_link == root) {

            if (cur_node == root || cur_node->parent == root) {
                cur_node->suff_link = root;
            } else {
                cur_node->suff_link = get_kid (get_suff_link (cur_node->parent), cur_node->last_symbol);
            }
        }

        return cur_node->suff_link;
    }

    std::shared_ptr<Node> get_kid (const std::shared_ptr<Node>& cur_node, char symbol) {


        if (cur_node->kids.find (symbol) == cur_node->kids.end () || cur_node->kids[symbol] == root) {

            if (cur_node->go.find (symbol) != cur_node->go.end () && cur_node->go[symbol] != root) {

                cur_node->kids[symbol] = cur_node->go[symbol];
            } else if (/* cur_node->kids.find (symbol) == cur_node->kids.end () */cur_node == root) {

                    cur_node->kids[symbol] = root;
                } else {

                    cur_node->kids[symbol] = get_kid (get_suff_link (cur_node), symbol);
                }
        }

        return cur_node->kids[symbol];
    }

    std::shared_ptr<Node> get_condenced_suff_link (const std::shared_ptr<Node>& cur_node) {
        if (cur_node->condenced_suff_link == root) {

            auto u = get_suff_link (cur_node);
            if (u == root) {

                cur_node->condenced_suff_link = root;
            } else {

                cur_node->condenced_suff_link = (u->is_terminal ? u : get_suff_link (u));
            }
        }
        return cur_node->condenced_suff_link;
    }

};

class TaskE {

    std::string str;
    std::size_t N;
    std::vector<std::vector<int>> ans;
    
    std::vector<std::string> vocabulary;

public:
    TaskE () = default;
    void solve ();


    friend std::istream& operator>> (std::istream &in, TaskE &task);
    friend std::ostream& operator<< (std::ostream &out, const TaskE &task);
};

std::istream &operator>> (std::istream &in, TaskE &task)
{
    in >> task.str >> task.N;
    task.vocabulary.resize (task.N);
    for (auto& i : task.vocabulary) {
        in >> i;
    }
    return in;
}

void TaskE::solve ()
{
    Trie trie;

    {
        int k = 0;
        for (auto &i : vocabulary)
        {
            trie.add_str (i, k++);
        }
    }

    int str_size = str.size ();
    auto node = trie.root;
    ans.resize (N);
    for (int i = 0; i < str_size; i++) {
        node = trie.get_kid (node, str[i]);

        for (auto k = node; k != trie.root; k = trie.get_condenced_suff_link (k)) {
            if (k->is_terminal) {
                for (const auto& m : k->str_numbers) {
                    ans[m].push_back (i + 2 - vocabulary[m].size ());
                }
            }
        }
    }


}

std::ostream &operator<< (std::ostream &out, const TaskE &task)
{
    for (const auto& i : task.ans) {
        out << i.size () << " ";
        for (const auto& k : i) {
            out << k << " ";
        }
        out << "\n";
    }
    return out;
}

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    TaskE task;
    std::cin >> task;
    task.solve ();
    std::cout << task;
}
