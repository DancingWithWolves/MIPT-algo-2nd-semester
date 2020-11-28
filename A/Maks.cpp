
#include <iostream>
#include <string>
 
 
 
int main() {
    std::string str_1, str_2;
    std::cin >> str_1 >> str_2;
 
    for (int i = 0; i <= str_1.size() - str_2.size(); ++i) {
        if (str_1.compare(i, str_2.size(), str_2) == 0) {
            std::cout << i << " ";
        }
    }
 
    return 0;
}