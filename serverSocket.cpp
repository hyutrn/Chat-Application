#include "serverSocket.h"
#include <unordered_map>

std::vector<Account> accounts;
std::vector<SOCKET> clients;
std::unordered_map<std::string, bool> loggedInAccounts;                 // Dùng để quản lý login logout của account
std::mutex clientsMutex;
std::mutex accountsMutex;
std::mutex loginMutex;                                                  // Mutex mới để quản lý trạng thái đăng nhập
int currentID = 1;                                                      // Biến toàn cục để theo dõi ID hiện tại

// Hàm tạo account khi nhận được key = 0 từ chuỗi tin nhắn
void CreateAccount(const std::string& username, const std::string& password) {
    // Tạo account mới với id
    Account newAccount = {username, password, currentID++};
    accounts.push_back(newAccount);
    std::cout << "Account created: " << username << " with ID: " << newAccount.id << std::endl;
}

// Hàm kiểm tra tài khoản có trong server ?
bool CheckAccount(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(accountsMutex);
    for (const auto& account : accounts) {
        if (account.username == username && account.password == password) {
            return true;
        }
    }
    return false;
}

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

// Hàm handle cho client 
void HandleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvSize == SOCKET_ERROR) {
        std::cout << "Receive failed" << std::endl;
        closesocket(clientSocket);
        return;
    }

    // Kiểm tra chuỗi message, tách chuỗi thành username, password, key để xử lý
    std::string message(buffer, recvSize);
    size_t pos1 = message.find(',');
    size_t pos2 = message.rfind(',');

    if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == pos2) {
        std::cout << "Invalid message format" << std::endl;
        closesocket(clientSocket);
        return;
    }

    std::string username = message.substr(0, pos1);
    std::string password = message.substr(pos1 + 1, pos2 - pos1 - 1);
    int key = std::stoi(message.substr(pos2 + 1));

    // Với key = 0, gọi hàm tạo account và lưu account mới vào server
    if (key == 0) {
        CreateAccount(username, password);
        // Tạo account thành công, gửi mã 200 về client
        send(clientSocket, "200", 3, 0);

        //Kiểm tra đăng nhập sau khi tạo tài khoản
        while(recv(clientSocket, buffer, sizeof(buffer), 0)){
            std::string message(buffer, recvSize);
            size_t pos1 = message.find(',');
            size_t pos2 = message.rfind(',');
            if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == pos2) {
                std::cout << "Invalid message format" << std::endl;
                closesocket(clientSocket);
                return;
            }
            std::string username = message.substr(0, pos1);
            std::string password = message.substr(pos1 + 1, pos2 - pos1 - 1);

            std::lock_guard<std::mutex> lock(loginMutex); // Sử dụng mutex để bảo vệ truy cập vào loggedInAccounts
            // Kiểm tra account có trong hệ thống và không được đăng nhập, gửi mã 201 về client
            if (CheckAccount(username, password) && !loggedInAccounts[username]) {
                loggedInAccounts[username] = true;
                std::cout << "Account login: " << username << std::endl;
                send(clientSocket, "201", 3, 0);
                break;
            } else {
                std::cout << "Account login failed: " << username << std::endl;
                send(clientSocket, "401", 3, 0);
                closesocket(clientSocket);
                return;
            }
        }
    } else if (key == 1) {                              // Với key = 1, tiến hành đăng nhập account, kiểm tra account có tồn tại ?
        std::lock_guard<std::mutex> lock(loginMutex); // Sử dụng mutex để bảo vệ truy cập vào loggedInAccounts
        // Kiểm tra account có trong hệ thống và không được đăng nhập, gửi mã 201 về client
        if (CheckAccount(username, password) && !loggedInAccounts[username]) {
            loggedInAccounts[username] = true;
            std::cout << "Account login: " << username << std::endl;
            send(clientSocket, "201", 3, 0);
        } else {
            std::cout << "Account login failed: " << username << std::endl;
            send(clientSocket, "401", 3, 0);
            closesocket(clientSocket);
            return;
        }
    } else {
        std::cout << "Invalid key value" << std::endl;      // Các trường hợp còn lại log lỗi ra màn hình
        closesocket(clientSocket);
        return;
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
            //Với câu lệnh "./exit", tức client sẽ ngắt kết nối với server
            if (clientMessage == "./exit") {
                std::cout << "Account disconnected: " << username << std::endl;
                break;
            } else {
                BroadcastMessage(clientMessage, clientSocket, username); //gọi hàm gửi tin nhắn cho các client khác
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

    LogoutAccount(username); // Đánh dấu tài khoản là đã đăng xuất
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
