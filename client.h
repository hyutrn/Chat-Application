#include <winsock2.h>
#include "GUI.h"
#include <vector>
#include <thread>
#include <set>
#include <regex> // Thêm thư viện regex để kiểm tra mật khẩu
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")

const char *IP_SERVER = "192.168.37.110";

#define PORT 8080

std::string keyEncrypt = "hello";       

std::vector<int> initMenuColor = {};
std::set<std::string> roomAvailable;
std::string chatDatabase = "";
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

struct client{
    std::string username = "";
    std::string password = "";
};

bool isPasswordValid(const std::string& password);
std::string createLabel(const std::string& roomName, size_t size = 50, char fillChar = '=');
std::string labelUser(const std::string &line, const std::string &username);
void GENERATE_LOGIN();
void ENTER_ROOM();
void SELECT_ROOM();
void GENERATE_ROOM();
void JOIN_CHAT(std::string roomName);
void GENERATE_SIGNUP();
void FIRST_MENU();
int SOCKET_START(SOCKET &clientSocket);
void AUTHENTICATION();
void SET_UP_CONNECTION();
void RESET_SOCKET();
void IP_CONFIG();
void LOAD_HISTORY_CHAT(std::string &chatDatabase, std::string &username);
void REQUEST_TO_SERVER(std::string &chatDatabase, std::string roomName);

void receiveMessages(SOCKET clientSocket);
