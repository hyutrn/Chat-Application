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
#include <fstream>
#include <set>

#pragma comment(lib, "Ws2_32.lib")

#define PORT_SERVER 8080
#define IP_SERVER "192.168.37.110"

#define FILE_NAME "account.txt"
std::string keyEcrypt = "hello";

struct Account {
    std::string username;
    std::string password;
    int key;
};

std::set<std::string> roomAvailable;
std::string encryptMessage(const std::string& plaintext, const std::string& key);
bool CheckID(const std::string& ID, const std::string& fileName);
int generateID();
void logAccount(const std::string& username, int flagAccount);
int insertAccount(const std::string& userName, const std::string& passWord, const int ID, const std::string& fileName, SOCKET clientSocket);
int CreateAccount(const std::string& username, const std::string& password);
bool CheckAccount(const std::string& userName, const std::string password,  const std::string& fileName);
Account CheckMessage(const std::string& message);
void BroadcastMessage(const std::string& message, SOCKET senderSocket, const std::string& username);
void HandleClient(SOCKET clientSocket);
int StartServer();

#endif // SERVER_H
