#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unordered_map>
#include<fstream>

#pragma comment(lib, "Ws2_32.lib")

#define PORT_SERVER 8080
#define IP_SERVER "192.168.37.41"

#define FILE_NAME "account.txt"

struct MessageAccount {
    std::string username;
    std::string password;
    int key;
};


int CreateAccount(const std::string& username, const std::string& password);
bool CheckAccount(const std::string& userName, const std::string password,  const std::string& fileName);
void BroadcastMessage(const std::string& message, SOCKET senderSocket, const std::string& username);
void HandleClient(SOCKET clientSocket);
int StartServer();

#endif // SERVER_H
