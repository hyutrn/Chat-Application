#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unordered_map>
#include<fstream>
#include <cstdlib> // thư viện cần thiết cho rand() và srand()
#include <ctime>   // thư viện cần thiết cho time()
#include <set>

#pragma comment(lib, "Ws2_32.lib")

#define PORT_SERVER 8080

#define FILE_NAME "account.txt"
std::string keyEcrypt = "hello";
std::string keyFile = "123";

std::string ipServer = "192.168.37.110";

struct Account {
    std::string username;
    std::string password;
    int key;
};

struct Message
{
    std::string roomName;
    std::string userName;
    std::string message;
};
std::set<std::string> roomAvailable;

// List of user
void listofUser(const std::string&message, SOCKET senderSocket, const std::string& username);

// Enscrypt and descrypt message
std::string encryptMessage(const std::string& plaintext, const std::string& key);
std::string decryptMessage(const std::string& ciphertext, const std::string& key);

// Generate, check ID for user
bool CheckID(const std::string& ID, const std::string& fileName);
int generateID();

// Handle login, sign up and log out
void logAccount(const std::string& username, int flagAccount);
int insertAccount(const std::string& userName, const std::string& passWord, const int ID, const std::string& fileName, SOCKET clientSocket);
int CreateAccount(const std::string& username, const std::string& password);
bool CheckAccount(const std::string& userName, const std::string password,  const std::string& fileName);
bool CheckLogIn(const std::string& username, const std::string& password, const std::string& fileName);
Account CheckMessage(const std::string& message);

// Broadcast message
void BroadcastMessage(const std::string& message, SOCKET senderSocket, const std::string& username);

/*
// Read, write log chat in room
void writeChat();
void loadChat();
*/

// Handl client
void HandleClient(SOCKET clientSocket);

// Config server
int StartServer();

#endif // SERVER_H
