#include <iostream>
#include <fstream>
#include <vector>

#include "SearchPassword.h"

using std::cout;

int main(int argc, char* argv[]) {
    std::string filename;

    std::cout << "Enter the file name where pwned-passwords-sha1-ordered-by-hash is stored:\n";
    std::cin >> filename;

    SearchPassword searchPassword(filename);

    std::cout << "Enter password to search\n";
    std::string passwordToSearch;
    std::cin >> passwordToSearch;

    while(passwordToSearch != "]^") {
        std::string result = searchPassword.binarySearch(passwordToSearch);
        if (result == "-1") {
            std::cout << ">>>>>>Good News! Password not found\n\n";
        } else {
            std::cout << ">>>>>>Oh no — pwned!\n";
            std::cout << ">>>>>>This password has been seen " << result << " times before\n\n";
        }
        std::cin >> passwordToSearch;
    }
}