#include <winsock2.h>
#include "GUI.h"
#include <vector>
#include <thread>
#include <set>
#include <regex> // Thêm thư viện regex để kiểm tra mật khẩu
#pragma comment(lib, "Ws2_32.lib")

const char *IP_SERVER = "192.168.37.110";

#define PORT 8080

std::string keyEncrypt = "hello";       

std::vector<int> initMenuColor = {};
std::set<std::string> roomAvailable;
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

struct client{
    std::string username = "";
    std::string password = "";
};

bool isPasswordValid(const std::string& password);
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

void receiveMessages(SOCKET clientSocket);
