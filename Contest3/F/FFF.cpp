#include <iostream>
#include <set>
#include <map>
#include <string>
 
using namespace std;
typedef unsigned long long ull;
/*
10 10
12
ADD 1 1
ADD 2 1
ADD 3 1
ADD 1 2
ADD 4 2
ADD 5 2
ADD 4 3
ADD 1 4
ADD 2 4
DELETE 1 4
DELETE 2 4
LISTSETSOF 1
 
*/
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ull N, number;
    int M, K, set_;
    cin >> N >> M >> K;
 
    map <ull, set<int>> my_map;
    set <ull> *sets = new set <ull> [M+1];
 
    string command = "";
    for (int i = 0; i < K; ++i) {
        cin >> command;
 
        if (command[0] == 'A') { //add
 
            cin >> number >> set_;
 
            sets[set_].insert(number);
            my_map[number].insert(set_);
 
        }
        else if (command[0] == 'D') { //delete
 
            cin >> number >> set_;
            sets[set_].erase(sets[set_].find(number));
            my_map[number].erase(set_);
 
        }
        else if (command[0] == 'C') { //clear
 
            cin >> set_;
            for (auto it = my_map.begin(); it != my_map.end(); ++it ) {
                it->second.erase(set_);
            }
            sets[set_].clear();
 
        }
        else if (command.size() == 7) { //LISTSET
 
            cin >> set_;
            if (sets[set_].size() == 0)
                cout << "-1\n";
            else {
                for (auto i = sets[set_].begin(); i != sets[set_].end(); ++i) {
                    cout << *i << " ";
                }
                cout << "\n";
            }
 
        } else { //LISTSETSOF
 
            cin >> number;
            bool somethingFound = false;
 
            if (my_map.find(number) != my_map.end() || my_map.find(number)->second.size() == 0) {
                set<int> cur = my_map[number];
                for (auto it = cur.begin(); it != cur.end(); ++it ) {
                    somethingFound = true;
                    cout << *it << " ";
                }
 
            }
            cout << (somethingFound ? "\n" : "-1\n");
        }
    }
    delete[] sets;
}