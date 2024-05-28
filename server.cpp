#include "server.h"
#include <unordered_map>

std::vector<SOCKET> clients;
std::unordered_map<std::string, bool> loggedInAccounts;                 // Dùng để quản lý login logout của account
std::mutex clientsMutex;
std::mutex accountsMutex;
std::mutex loginMutex;                                                  // Mutex mới để quản lý trạng thái đăng nhập
int currentID = 1;                                                      // Biến toàn cục để theo dõi ID hiện tại


/*
*   Find Account already exit or not
*   return true if username already exit
*   else return false 
*/
bool CheckAccount(const std::string& userName, const std::string& fileName)
{
    std::ifstream file(fileName);

    if(!file.is_open())
    {
        std::cout<<"Error when opening file\n"; 
        return false;
    }
    std::string line; 
    while (std::getline(file, line)) {
        if (line.substr(0, userName.length()) == userName) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}


/*
*   Save account in file
*   return -1 if account already exit
*   return -2 if file cannot open
*   return 1 save successfully
*/
int insertAccount(const std::string& userName, const std::string& passWord, const int ID, const std::string& fileName, SOCKET clientSocket)
{
    if(CheckAccount(userName, FILE_NAME) == true)
    {

        std::cout<<"Username already exits\n";
        if(send(clientSocket, "403", 3, 0) < 0)
        {
            std::cout<<"Error when send message"<<std::endl;
        }
        return -1;
    }
    std::ofstream file(fileName, std::ios::app); 

    if(!file.is_open())
    {
        std::cout<<"Error when opening file\n"; 
        return -2;
    }
    file << userName<<","<<passWord<<","<<ID<<std::endl;
    

    file.close();
    std::cout<<"Save Successfully\n";

    return 1;
}


// Hàm tạo account khi nhận được key = 0 từ chuỗi tin nhắn
int CreateAccount(const std::string& username, const std::string& password, SOCKET clientSocket) {
   
    return insertAccount(username, password, currentID++, FILE_NAME,clientSocket);
}

// Hàm kiểm tra tài khoản có trong server ?
// Hàm logout accout
void LogoutAccount(const std::string& username) {
    std::lock_guard<std::mutex> lock(accountsMutex);
    loggedInAccounts[username] = false;
}

// Hàm gửi tin nhắn tới các user khác trong room chat
void BroadcastMessage(const std::string& message, SOCKET senderSocket, const std::string& username) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::string senderName = username;                              // Lấy tên người gửi
    std::string fullMessage = senderName + ": " + message;          // Tạo tin nhắn đầy đủ
    std::cout << fullMessage << std::endl;
    for (SOCKET client : clients) { 
        if (client != senderSocket) {                               //Gửi cho các user khác
            send(client, fullMessage.c_str(), fullMessage.length(), 0);
        }
    }
}
/*
*  Kiểm tra chuỗi message, tách chuỗi thành username, password, key để xử lý
*  return username, password, key
*/
MessageAccount CheckMessage(const std::string& message)
{
    size_t pos1 = message.find(',');
    size_t pos2 = message.rfind(',');
    if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == pos2) {
        throw std::invalid_argument("Invalid message format");
    }
    std::string username = message.substr(0, pos1);
    std::string password = message.substr(pos1 + 1, pos2 - pos1 - 1);
    int key = std::stoi(message.substr(pos2 + 1));

    return {username, password, key};
}

// Hàm handle cho client 
void HandleClient(SOCKET clientSocket) {
    
    char buffer[1024] = {0};
    std::string UserName;
    int recvSize;
    // int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    // if (recvSize == SOCKET_ERROR) {
    //     std::cout << "Receive failed" << std::endl;
    //     closesocket(clientSocket);
    //     return;
    // }
    // std::string message(buffer, recvSize);
    // // std::cout<<message<<std::endl;
    // MessageAccount Messsage = CheckMessage(message);

    while(recvSize = recv(clientSocket, buffer, sizeof(buffer), 0))
    {
        //int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::string message(buffer, recvSize); 
        MessageAccount messageLogin = CheckMessage(message);
        if (messageLogin.key == 0) {
            if(CreateAccount(messageLogin.username, messageLogin.password,clientSocket) > 0)
            {
                send(clientSocket, "200", 3, 0);
            }
        }
        else if(messageLogin.key == 1)
        {
            if (CheckAccount(messageLogin.username, FILE_NAME)) {
                loggedInAccounts[messageLogin.username] = true;
                UserName = messageLogin.username;
                std::cout << "Account login: " << messageLogin.username << std::endl;
                send(clientSocket, "201", 3, 0);
                break;
            } else {
                std::cout << "Account login failed: " << messageLogin.username << std::endl;
                send(clientSocket, "401", 3, 0);
                // closesocket(clientSocket);
                // return;
            }
        }
        else {
               std::cout << "Invalid key value" << std::endl;      // Các trường hợp còn lại log lỗi ra màn hình
                closesocket(clientSocket);
            return;
        }
    }

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(clientSocket);
    }

    // Xử lý phần gửi nhận tin nhắn
    while (true) {

        recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvSize > 0) {
             std::string clientMessage(buffer, recvSize);
             std::cout<<clientMessage<<std::endl;
            //Với câu lệnh "./exit", tức client sẽ ngắt kết nối với server
            if (clientMessage == "./exit") {
                break;
            } else {
                 BroadcastMessage(clientMessage, clientSocket, UserName); //gọi hàm gửi tin nhắn cho các client khác
            }
        } else {
            // Lock the mutex before erasing from clients vector
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                auto it = std::remove_if(clients.begin(), clients.end(), [clientSocket](SOCKET s) {
                    return s == clientSocket;
                });
                clients.erase(it, clients.end());
            }

            closesocket(clientSocket);
            break;
        }
    }

    // Remove the client from the clients list and close the socket
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }

    LogoutAccount(UserName); // Đánh dấu tài khoản là đã đăng xuất
    closesocket(clientSocket); 
}

// Hàm khởi động Servere
int StartServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    serverAddr.sin_port = htons(PORT_SERVER);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT_SERVER << "..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Accept failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::thread clientThread(HandleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


// Hàm main
int main() {
    return StartServer();
}
