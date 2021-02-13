# Have-i-been-Pawned
C++ application similar to https://haveibeenpwned.com/Passwords. 

The application takes in a password and gives a count of how many times that password had been seen in the source data breaches. The list of sha1 passwords(ordered by hash) can be downloaded from https://downloads.pwnedpasswords.com/passwords/pwned-passwords-sha1-ordered-by-hash-v7.7z.

The application makes use of OpenSSL libary to calculate the SHA1 of the password provided as an input. The SHA1 calculated using OpenSSL is then searched through the huge file downloaded before. The application makes use of binary search to search through the huge file.

To exit the application enter "]^" (without quotes of course).

To compile the code:
g++ -o main -Wall -Wunused main.cpp -lssl -lcrypto
