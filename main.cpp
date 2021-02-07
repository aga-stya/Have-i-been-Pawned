#include <iostream>
#include <fstream>
#include <vector>

#include "SearchPassword.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename;

    cout << "Enter the file name:\n";
    cin >> filename;

    SearchPassword searchPassword(filename);

    cout << "Enter password to search\n";
    string passwordToSearch;
    cin >> passwordToSearch;

    while(passwordToSearch != "exit") {
        string result = searchPassword.binarySearch(passwordToSearch);
        if (result == "-1")
            cout << "Password not found\n";
        else
            cout << "Password found. Used for " << result << " times.\n";
        cin >> passwordToSearch;
    }
}