#include <winsock2.h>
#include "GUI.h"
#include <vector>
#include <thread>
#include <string>   
#pragma comment(lib, "Ws2_32.lib")

#define IP_SERVER "192.168.37.44"
#define PORT 8080
std::string keyEncrypt = "hello";       

std::vector<int> initMenuColor = {};
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

struct client{
    std::string username = "";
    std::string password = "";
};

std::string encryptMessage(const std::string& plaintext, const std::string& key);
std::string decryptMessage(const std::string& ciphertext, const std::string& key);
void GENERATE_LOGIN();
void JOIN_CHAT();
void GENERATE_SIGNUP();
void FIRST_MENU();
int SOCKET_START(SOCKET &clientSocket);
void AUTHENTICATION();
void SET_UP_CONNECTION();
void RESET_SOCKET();

void receiveMessages(SOCKET clientSocket);
