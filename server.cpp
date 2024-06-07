#include "server.h"

std::vector<SOCKET> clients;
std::unordered_map<std::string, bool> loggedInAccounts;         // Dùng để quản lý login logout của account
std::mutex clientsMutex;
std::mutex accountsMutex;
std::mutex loginMutex;                                          // Mutex mới để quản lý trạng thái đăng nhập
int currentID;                                              // Biến toàn cục để theo dõi ID hiện tại

// Hàm thực hiện mã hóa
std::string encryptMessage(const std::string& plaintext, const std::string& key){
    std::string ciphertext = plaintext; // Khởi tạo chuỗi mã hóa với cùng độ dài như plaintext
    
    for (size_t i = 0; i < plaintext.size(); ++i) {
        ciphertext[i] = plaintext[i] ^ key[i % key.size()]; // XOR từng ký tự của plaintext với key
    }
    
    return ciphertext; 
}

// Hàm thực hiện giải mã
std::string decryptMessage(const std::string& ciphertext, const std::string& key){
    std::string decryptedtext = ciphertext; // Khởi tạo chuỗi giải mã với cùng độ dài như ciphertext
    
    for (size_t i = 0; i < ciphertext.size(); ++i) {
        decryptedtext[i] = ciphertext[i] ^ key[i % key.size()]; // XOR từng ký tự của ciphertext với key
    }
    
    return decryptedtext;
}  

// Hàm kiểm tra ID trong file
bool CheckID(const std::string& ID, const std::string& fileName){
    std::ifstream file(fileName);
    if (!file.is_open()){
        std::cout << "Error when opening file\n";
        return false;
    }
    std::string line;
    while (std::getline(file, line)){
        std::stringstream ss(line);
        std::string username, password, id;
        // Đọc dữ liệu theo định dạng username,password,ID
        if (std::getline(ss, username, ',') && std::getline(ss, password, ',') && std::getline(ss, id, ',')){
            // Kiểm tra ID
            if (id == ID){
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

// Hàm khởi tạo ID cho user
int generateID() {
    // Khởi tạo bộ sinh số ngẫu nhiên chỉ một lần
    static bool seedInitialized = false;
    if (!seedInitialized) {
        std::srand(std::time(0));
        seedInitialized = true;
    }

    std::string newID;
    do {
        int ID = 1000 + std::rand() % 9000;
        newID = std::to_string(ID);
    } while (CheckID(newID, FILE_NAME));

    return std::stoi(newID);
}

/*
*   Find Account already exit or not
*   return true if username already exit
*   else return false 
*/
bool CheckAccount(const std::string& userName, const std::string& fileName){
    std::ifstream file(fileName);
    if(!file.is_open()){
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

/**
 * Function check password when login
 * return true if match password else return false
*/
bool CheckLogIn(const std::string& username, const std::string& password, const std::string& fileName)
{
    std::ifstream file(fileName);

    if(!file.is_open())
    {
        std::cout<<"Error when opening file\n";
        return false;
    }

    std::string line;
    while(std::getline(file, line))
    {
        if(line.substr(0, username.length()) == username)
        {
      
            if(line.substr(username.length() + 1, password.length()) == password)
            {
                std::cout<<"Password correct";
                file.close();
                return true;
            }
            
        }
    }
    std::cout<<"Password Invalid";
    return false;
}

/*
*   Save account in file
*   return -1 if account already exit
*   return -2 if file cannot open
*   return 1 save successfully
*/
int insertAccount(const std::string& userName, const std::string& passWord, const int ID, const std::string& fileName, SOCKET clientSocket){
    // Kiem tra user da ton tai trong he thong ?
    if(CheckAccount(userName, FILE_NAME) == true){
        std::cout<<"Username already exits\n";
        // Gui ma loi 403 thong bao cho user username da ton tai
        if(send(clientSocket, "403", 3, 0) < 0){
            std::cout<<"Error when send message"<<std::endl;
        }
        return -1;
    }
    std::ofstream file(fileName, std::ios::app); 
    if(!file.is_open()){
        std::cout<<"Error when opening file\n"; 
        return -2;
    }
    file << userName<<","<<passWord<<","<<ID<<std::endl;
    file.close();
    std::cout<<"Save Successfully\n";
    return 1;
}

// Hàm tạo account khi nhận được key = 0 từ chuỗi tin nhắn
int CreateAccount(const std::string& username, const std::string& password, SOCKET clientSocket){
    // Khoi tao ID cho user
    currentID = generateID();
    // Goi ham insertAccount de them user vao he thong
    return insertAccount(username, password, currentID, FILE_NAME,clientSocket);
}

// Hàm theo doi login logout cua accout
// Khi account login vao server, flag = 1
// Khi account logout khoi server, flag = 0
void logAccount(const std::string& username, int flagAccount){
    if(flagAccount == 0){
        std::lock_guard<std::mutex> lock(accountsMutex);
        loggedInAccounts[username] = true;
    } 
    if(flagAccount == 1){
        std::lock_guard<std::mutex> lock(accountsMutex);
        loggedInAccounts[username] = false;        
    }
}

// Hàm gửi tin nhắn tới các user khác trong room chat
void BroadcastMessage(const std::string& message, SOCKET senderSocket, const std::string& username) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    //std::string senderName = username;                              // Lấy tên người gửi
    //std::string fullMessage = senderName + ": " + message;          // Tạo tin nhắn đầy đủ
    std::cout << message << std::endl;
    // Gui tin nhan cho cac user khac
    for (SOCKET client : clients) { 
        if (client != senderSocket) {                               
            send(client, message.c_str(), message.length(), 0);
        }
    }
}

/*
*  Kiểm tra chuỗi message, tách chuỗi thành username, password, key để xử lý
*  return username, password, key
*/
Account CheckMessage(const std::string& message){
    size_t pos1 = message.find(',');
    size_t pos2 = message.rfind(',');
    if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == pos2){
        throw std::invalid_argument("Invalid message format");
    }
    std::string username = message.substr(0, pos1);
    std::string password = message.substr(pos1 + 1, pos2 - pos1 - 1);
    int key = std::stoi(message.substr(pos2 + 1));
    return {username, password, key};
}

// Hàm handle cac client 
void HandleClient(SOCKET clientSocket) {
    char buffer[2048] = {0};
    // Bien UserName de luu username tu user
    std::string UserName; 
    int recvSize;

    while(recvSize = recv(clientSocket, buffer, sizeof(buffer), 0)){
        std::string message(buffer, recvSize);
        // Kiem tra tin nhan, tach lay username, password va key
        Account messageLogin = CheckMessage(message); 

        // TH1: Key = 0: Server tien hanh tao tai khoan cho user
        if (messageLogin.key == 0){
            // Neu viec khoi tao thanh cong, gui ma 200 bao cho user
            if(CreateAccount(messageLogin.username, messageLogin.password,clientSocket) > 0){
                send(clientSocket, "200", 3, 0);
            }
        }

        // TH2: Key = 1: Server doi user dang nhap
        else if(messageLogin.key == 1){
            // Xac thuc tai khoan co trong he thong
            if (CheckLogIn(messageLogin.username, messageLogin.password, FILE_NAME) == true && loggedInAccounts[messageLogin.username] == false){
                // Neu thanh cong, lay user luu vao UserName phuc vu viec gui, nhan tin nhan
                UserName = messageLogin.username;
                std::cout << "Account login: " << messageLogin.username << std::endl;
                // Gui ma 201 thong bao thanh cong cho user
                send(clientSocket, "201", 3, 0);
                // Danh dau user da dang nhap
                logAccount(UserName, 0);
                break;
            } else{
                // Nguoc lai, gui ma 401 cho user thong bao dang nhap khong thanh cong
                std::cout << "Account login failed: " << messageLogin.username << std::endl;
                send(clientSocket, "401", 3, 0);
            }
        }

        // Các trường hợp còn lại in thông báo lỗi ra màn hình
        else {
                std::cout << "Invalid key value" << std::endl;      
                closesocket(clientSocket);
            return;
        }
    }

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(clientSocket);
    }

    bool join = true;
    // Xử lý phần gửi nhận tin nhắn
    while (true){
        //Notify User join the chat room
        if(join){
            // Tao thong bao user tham gia phong chat
            std::string joinMessage = UserName + " joined the chat room.";
            std::string messJoin = encryptMessage(joinMessage, keyEcrypt);
            for (SOCKET client : clients){ 
                /*
                if(client != clientSocket){   
                    send(client,joinMessage.c_str(),joinMessage.length(), 0);
                }          
                */
                send(client, messJoin.c_str(), messJoin.length(), 0);
            }
            join = false;
        }
        recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvSize > 0) {
            std::string clientMessage(buffer, recvSize);
            std::cout<<clientMessage<<std::endl;
            //Với câu lệnh "./exit", tức client sẽ ngắt kết nối với server
            //notify exit the chat room
            if (clientMessage == "./exit") {
                // Tao thong user thoat khoi phong chat
                std::string exitMessage = UserName + " exit the chat room";
                std::string messExit = encryptMessage(exitMessage, keyEcrypt);
                for (size_t i = 0; i < clients.size(); ++i){
                    if(clients[i] != clientSocket){
                        send(clients[i],messExit.c_str(),messExit.length(), 0);
                    }
                }  
                break;
            } 
            else {
                BroadcastMessage(clientMessage, clientSocket, UserName); //gọi hàm gửi tin nhắn cho các client khác
            }
        }
        else {
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

    // Ghi nhan tai khoan da thoat, bat flag = 1
    logAccount(UserName, 1); 
    closesocket(clientSocket); 
}

// Hàm khởi động Servere
int StartServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET){
        std::cout << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }
    // Config cho socket server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    serverAddr.sin_port = htons(PORT_SERVER);
    // Bind server
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
        std::cout << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // Lang nghe cac ket noi tu user
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        std::cout << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening on port " << PORT_SERVER << "..." << std::endl;
    while (true){
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET){
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
