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

#pragma comment(lib, "Ws2_32.lib")

#define PORT_SERVER 8080
#define IP_SERVER "192.168.37.44"

struct Account {
    std::string username;
    std::string password;
    int id;
};

void CreateAccount(const std::string& username, const std::string& password);
bool CheckAccount(const std::string& username, const std::string& password);
void BroadcastMessage(const std::string& message, SOCKET senderSocket);
void HandleClient(SOCKET clientSocket);
int StartServer();

#endif // SERVER_H
