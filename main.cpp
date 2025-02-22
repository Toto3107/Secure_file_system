#include <iostream>
#include "auth.h"
#include "crypto.h"

using namespace std;

void showInitialMenu() {
    cout << "\n🔒 Secure File System\n";
    cout << "-----------------------\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

void showUserMenu() {
    cout << "\n🔐 File Encryption System\n";
    cout << "-------------------------\n";
    cout << "1. Encrypt a File\n";
    cout << "2. Decrypt a File\n";
    cout << "3. Logout\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    bool loggedIn = false;

    while (true) {
        if (!loggedIn) {
            showInitialMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loggedIn = loginUser();
                    break;
                case 3:
                    cout << "🔴 Exiting program.\n";
                    return 0;
                default:
                    cout << "❌ Invalid choice. Try again!\n";
            }
        } else {
            showUserMenu();
            cin >> choice;

            switch (choice) {
                case 1: {
                    string filename;
                    cout << "Enter filename to encrypt: ";
                    cin >> filename;
                    encryptFile(filename);
                    break;
                }
                case 2: {
                    string filename;
                    cout << "Enter filename to decrypt: ";
                    cin >> filename;
                    decryptFile(filename);
                    break;
                }
                case 3:
                    cout << "🔓 Logged out successfully!\n";
                    loggedIn = false;
                    break;
                default:
                    cout << "❌ Invalid choice. Try again!\n";
            }
        }
    }
}
