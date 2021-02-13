#ifndef SEARCH_PASSWORD_H
#define SEARCH_PASSWORD_H

#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <openssl/sha.h>
#include <algorithm>

namespace fs = std::filesystem;

class SearchPassword {
private:
    std::ifstream input;
    long long filesize;
    const int MAX_STRING_SIZE = 59; // Maximum size of the record
    const int MIN_STRING_SIZE = 42; // Minimum size of the record 

    // look into the file
    void seekAndRead(char* buffer, int size, long long position) {
        input.seekg(position, std::ios::beg);
        if (!input.read(buffer, size))
            std::cout << "Read failed at position:" << position << "\n";
    }

    // calculates the SHA1 of the password to be searched
    std::string calculateSHA1(std::string str) {
        unsigned char obuf[20]; // ssh value has 20 bytes
        SHA1(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(str.c_str())), str.size(), obuf);

        // Transform byte-array to string
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (const unsigned char &byte: obuf) {
            ss << std::setw(2) << (int)byte;
        }

        // convert the string to upper case
        std::string result= ss.str();
        std::for_each(begin(result), end(result),
                      [](char& c) { 
                          if (isalpha(c))
                            c = std::toupper(c);
                      });
        return result;
    }

public:
    SearchPassword(std::string file): input(file, std::ifstream::ate | std::ifstream::binary) {
        const auto fsize = fs::file_size(file);
        filesize = fsize;
        std::cout << "filesize:" << filesize << '\n';
    } 

    void reinitializeBuffer(char* arr, int size) {
        for (auto i = 0; i < size; i++)
            arr[i] = -1;
    }

    std::string binarySearch(std::string searchPassword) {
        if (!input.is_open()) {
            std::cout << "Cannot search, file is not open\n";
            return "-1";
        }
        if (searchPassword.empty()) {
            std::cout << "password is empty\n";
            return "-1";
        }

        std::string passwordSHA1 = calculateSHA1(searchPassword);

        std::cout << "SHA1 of the password:" << passwordSHA1 << "\n";

        long long start = 0;
        long long end = filesize - 1;
        long long mid = (start + end) / 2;
        char recordBuffer[MAX_STRING_SIZE];

        while((end - start) >= MIN_STRING_SIZE) {
            mid = (start + end) / 2;
            // go back till a '\n' is found or its the first record
            char newLine;
            long long startOfTheRecord = mid;
            seekAndRead(&newLine, 1, startOfTheRecord);
            while (newLine != '\n' && startOfTheRecord > 0) {
                startOfTheRecord = startOfTheRecord - 1;
                seekAndRead(&newLine, 1, startOfTheRecord);
            }
            if (startOfTheRecord != 0) // the check is for the first record
                startOfTheRecord++;

            // Read the record with max size
            long long bufferSize = MAX_STRING_SIZE;
            if (startOfTheRecord + MAX_STRING_SIZE > filesize)
                bufferSize = filesize - startOfTheRecord;
            seekAndRead(recordBuffer, bufferSize, startOfTheRecord);

            // separate the password and times used
            long long endOfTheRecord = startOfTheRecord;
            int itr = 0;
            std::string password;
            while (recordBuffer[itr] != ':') {
                password.append(1, recordBuffer[itr]);
                itr++;
                endOfTheRecord++;
            }
            itr++; // skip the ':'
            std::string timesUsed;
            while (recordBuffer[itr] != '\n') {
                timesUsed.append(1, recordBuffer[itr]);
                itr++;
                endOfTheRecord++;
            }
            timesUsed = timesUsed.substr(0, timesUsed.size() - 1); // remove the last character which will be '\r'

            // normal binary search
            if (password == passwordSHA1) {
                return timesUsed;
            } else if (password > passwordSHA1) {
                end = startOfTheRecord - 1;
            } else {
                start = endOfTheRecord + 1;
            }
        }
        return "-1";
    }
};
#endif
