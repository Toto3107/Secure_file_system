#include "crypto.h"
#include <fstream>
#include <vector>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/rand.h>

// Function to generate a random AES key
void generateAESKey(unsigned char *key, int size) {
    RAND_bytes(key, size);
}

// Function to encrypt a file
void encryptFile(const string &filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file!" << endl;
        return;
    }

    string encryptedFile = filename + ".enc";
    ofstream outFile(encryptedFile, ios::binary);
    if (!outFile) {
        cout << "Error creating encrypted file!" << endl;
        return;
    }

    unsigned char key[32];
    unsigned char iv[16];
    generateAESKey(key, 32);
    RAND_bytes(iv, 16);

    outFile.write(reinterpret_cast<char *>(iv), 16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    vector<unsigned char> buffer(1024);
    vector<unsigned char> encrypted(1032);

    int bytesRead, outLen;
    while ((bytesRead = inFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()).gcount()) > 0) {
        EVP_EncryptUpdate(ctx, encrypted.data(), &outLen, buffer.data(), bytesRead);
        outFile.write(reinterpret_cast<char *>(encrypted.data()), outLen);
    }
    EVP_EncryptFinal_ex(ctx, encrypted.data(), &outLen);
    outFile.write(reinterpret_cast<char *>(encrypted.data()), outLen);

    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();

    cout << "✅ File encrypted successfully: " << encryptedFile << endl;
}

// Function to decrypt a file
void decryptFile(const string &encryptedFilename) {
    ifstream inFile(encryptedFilename, ios::binary);
    if (!inFile) {
        cout << "Error opening encrypted file!" << endl;
        return;
    }

    string decryptedFile = encryptedFilename.substr(0, encryptedFilename.size() - 4);
    ofstream outFile(decryptedFile, ios::binary);
    if (!outFile) {
        cout << "Error creating decrypted file!" << endl;
        return;
    }

    unsigned char key[32];
    unsigned char iv[16];
    inFile.read(reinterpret_cast<char *>(iv), 16);

    generateAESKey(key, 32);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    vector<unsigned char> buffer(1032);
    vector<unsigned char> decrypted(1024);

    int bytesRead, outLen;
    while ((bytesRead = inFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()).gcount()) > 0) {
        EVP_DecryptUpdate(ctx, decrypted.data(), &outLen, buffer.data(), bytesRead);
        outFile.write(reinterpret_cast<char *>(decrypted.data()), outLen);
    }
    EVP_DecryptFinal_ex(ctx, decrypted.data(), &outLen);
    outFile.write(reinterpret_cast<char *>(decrypted.data()), outLen);

    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();

    cout << "✅ File decrypted successfully: " << decryptedFile << endl;
}
