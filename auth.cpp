#include "auth.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <openssl/sha.h>

// Function to generate SHA-256 hash
string sha256(const string &password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password.c_str(), password.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to register a user
void registerUser() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    string hashedPassword = sha256(password);
    ofstream file("users.txt", ios::app);

    if (file.is_open()) {
        file << username << " " << hashedPassword << endl;
        file.close();
        cout << "✅ User registered successfully!\n";
    } else {
        cout << "❌ Error: Unable to open user database!\n";
    }
}

// Function to verify user login
bool loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string hashedPassword = sha256(password);
    ifstream file("users.txt");
    string fileUser, filePass;

    if (file.is_open()) {
        while (file >> fileUser >> filePass) {
            if (fileUser == username && filePass == hashedPassword) {
                cout << "✅ Login successful!\n";
                return true;
            }
        }
        file.close();
    }

    cout << "❌ Invalid username or password!\n";
    return false;
}
