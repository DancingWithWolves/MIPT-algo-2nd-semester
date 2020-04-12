#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
 
using namespace std;
typedef long long ull;

int main()
{
    ull N, number, M, K, set_;
    //ifstream cin;
    //cin.open("input.txt");

    cin >> N >> M >> K;
 
    map<ull, set<ull>> my_map;
    map<ull, set<ull>> sets;

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
            sets[set_].erase(number);
            my_map[number].erase(set_);
 
        }
        else if (command[0] == 'C') { //clear
 
            cin >> set_;
            
            for (auto it = sets[set_].begin(); it != sets[set_].end(); ++it ) {
                my_map[*it].erase(set_);
            }
            sets[set_].clear();
 
        }
        else if (command.size() == 7) { //LISTSET
 
            cin >> set_;
            if (sets[set_].size() == 0) 
                cout << "-1\n";
            else {
                for (auto it = sets[set_].begin(); it != sets[set_].end(); ++it) {
                    cout << *it << " ";
                }
                cout << "\n";
            }
 
        } else { //LISTSETSOF
 
            cin >> number;

            if (my_map[number].size() == 0)
                cout << "-1";
            else {
                for (auto it = my_map[number].begin(); it != my_map[number].end(); ++it) {
                    cout << *it << " ";
                }
            }
            cout << "\n";
        }
    }
}