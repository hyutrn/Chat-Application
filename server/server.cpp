#include "server.h"

std::vector<SOCKET> clients;
std::unordered_map<std::string, bool> loggedInAccounts;         // Dùng để quản lý login logout của account
std::mutex clientsMutex;
std::mutex accountsMutex;
std::mutex loginMutex;                                          // Mutex mới để quản lý trạng thái đăng nhập
int currentID;                                              // Biến toàn cục để theo dõi ID hiện tại

std::unordered_map<std::string, std::vector<SOCKET>> chatroom;
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
        newID = std::to_string(ID)
;
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
        //Adding check encrypt data 
        std::string encryptName = encryptMessage(userName, keyFile);
        if (line.substr(0, userName.length()) == encryptName) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

//check password when user login with username return true when password correct
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
        //adding check encrypt data
        std::string encryptName = encryptMessage(username, keyFile);
        std::string encryptPass = encryptMessage(password, keyFile);
        if(line.substr(0, username.length()) == encryptName)
        {

            if(line.substr(username.length() + 1, password.length()) == encryptPass)
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

    // Ma hoa du lieu duoc ghi vao file
    std::string nameEncrypted = encryptMessage(userName, keyFile);
    std::string pwdEncypted = encryptMessage(passWord, keyFile);
    
    //file << userName<<","<<passWord<<","<<ID<<std::endl;
    file << nameEncrypted<<","<<pwdEncypted<<","<<ID<<std::endl;
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
// Khi account login vao server, flag = 0
// Khi account logout khoi server, flag = 1
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

Message splitMessage(const std::string& message)
{
    size_t pos1 = message.find(',');
    size_t pos2 = message.rfind(',');
   
    std::string roomName = message.substr(0, pos1);
    std::string userName = message.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string messages = message.substr(pos2 + 1, message.length() - pos2);
    
    return {roomName, userName, messages};
}

/**
 * 
 * @param room name, clientSocket IP address of client
 * 
*/
void roomChat(std::string room, SOCKET clientSocket)
{
    auto check = std::find(chatroom[room].begin(), chatroom[room].end(), clientSocket);
    if(check == chatroom[room].end())
    {
        chatroom[room].emplace_back(clientSocket);
    }else{
        std::cout<<"Client exist"<<std::endl;
    }
}

/**
 * Send notify when someone join room
*/
void notifyJoin(std::string username, const std::vector<SOCKET>& clients, const SOCKET senderSocket)
{
    std::string notify = "              " + username  +" is joining room";
    notify = encryptMessage(notify, keyEcrypt);
    for (SOCKET client : clients) { 
        if (client != senderSocket) {                               
            send(client, notify.c_str(), notify.length(), 0);
        }
    }    
}

/**
 * save room name when user create
*/
int saveRoom(std::string roomName)
{
    std::ofstream file("Room.txt", std::ios::app); 
    if(!file.is_open()){
        std::cout<<"Error when opening file\n"; 
        return -2;
    }
    file <<roomName<<std::endl;
    file.close();
    std::cout<<"Save Successfully\n";
    return 0;
}
/**
 * When server ON need to load all room in vecto
*/
int loadRoom(std::string fileName)
{
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout<<"Error when opening file\n"; 
        return -1;
    }
    std::string line; 
    while (std::getline(file, line)) {
       chatroom[line];
    }
    file.close();
    
    return 1;
}
/**
 * Check room exist or not
*/
bool checkRoom(std::string roomName)
{
   return chatroom.find(roomName) != chatroom.end();
}

/**
 * Send file Room to client to display
*/
// int sendRoomtoClient(SOCKET socketclient)
// {

// }
//Tach username
std::string extractUsername(const std::string& fullMessage) {
    size_t colonPos = fullMessage.find(':');
    if (colonPos == std::string::npos) {
        return ""; // Không tìm thấy dấu hai chấm, trả về chuỗi rỗng
    }
    return fullMessage.substr(0, colonPos); // Trả về phần username trước dấu hai chấm
}
//Tach message
std::string extractActualMessage(const std::string& fullMessage) {
    size_t colonPos = fullMessage.find(':');
    if (colonPos == std::string::npos) {
        return ""; // Không tìm thấy dấu hai chấm, trả về chuỗi rỗng
    }
    return fullMessage.substr(colonPos + 1); // Trả về phần tin nhắn sau dấu hai chấm
}

//Send message 
void BroadcastMessage(const std::string& message, const std::vector<SOCKET>& clients, const SOCKET senderSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::string senderMessage; 
    senderMessage = decryptMessage(message, keyEcrypt);
    Message messBroadCast = splitMessage(senderMessage);

    std::string extractedUsername = extractUsername(messBroadCast.message);
    //std::cout << "Extracted Username: " << extractedUsername << std::endl; // Debug

    std::string actualMessage = extractActualMessage(messBroadCast.message);
    //std::cout << "Actual Message:" << actualMessage << std::endl; // Debug
    std::string msgSend = encryptMessage(messBroadCast.message, keyEcrypt);

    //Với câu lệnh "./exit", tức client sẽ ngắt kết nối với server
    if(actualMessage == " /exit"){
        // Gui thong bao cho cac user khac
        std::string notify = "              " + extractedUsername  +" exit room";
        notify = encryptMessage(notify, keyEcrypt);
        for (SOCKET client : clients) { 
            if (client != senderSocket) {                               
                send(client, notify.c_str(), notify.length(), 0);
            }
        }
    } else {
        // Gui tin nhan cho cac user khac
        for (SOCKET client : clients) { 
            if (client != senderSocket) {                               
                send(client, msgSend.c_str(), msgSend.length(), 0);
            }
        }
    }
}


// Hàm handle cac client 
void HandleClient(SOCKET clientSocket) {
    char buffer[2048] = {0};
    // Bien UserName de luu username tu user
    std::string UserName; 
    int recvSize;
    bool loginSuccessful = false;
    try{
        while(recvSize = recv(clientSocket, buffer, sizeof(buffer), 0)){
            std::string message(buffer, recvSize);
            // Kiem tra tin nhan, tach lay username, password va key
            Account messageLogin = CheckMessage(message); 

            // TH1: Key = 0: Server tien hanh tao tai khoan cho user
            if (messageLogin.key == 0){
                // Neu viec khoi tao thanh cong, gui ma 200 bao cho user
                if(CreateAccount(messageLogin.username, messageLogin.password, clientSocket) > 0){
                    send(clientSocket, "200", 3, 0);
                }
                else{
                    std::cout << "HAHA" << std::endl;
                    continue;
                }
            }

            // TH2: Key = 1: Server doi user dang nhap
            else if(messageLogin.key == 1){
                // Xac thuc tai khoan co trong he thong
                if (CheckAccount(messageLogin.username, FILE_NAME) == true
                    &&CheckLogIn(messageLogin.username, messageLogin.password, FILE_NAME) == true
                    && loggedInAccounts[messageLogin.username] == false){
                    // Neu thanh cong, lay user luu vao UserName phuc vu viec gui, nhan tin nhan
                    UserName = messageLogin.username;
                    std::cout << "Account login: " << messageLogin.username << std::endl;
                    // Gui ma 201 thong bao thanh cong cho user
                    send(clientSocket, "201", 3, 0);
                    // Danh dau user da dang nhap
                    logAccount(UserName, 0);
					loginSuccessful = true;
					//break;
				} else{
					// Nguoc lai, gui ma 401 cho user thong bao dang nhap khong thanh cong
					std::cout << "Account login failed: " << messageLogin.username << std::endl;
					send(clientSocket, "401", 3, 0);
				}
            }
            // Các trường hợp còn lại in thông báo lỗi ra màn hìnhplitMessage
            else {
                std::cout << "Invalid key value" << std::endl;      
                closesocket(clientSocket);
                return;
            }

            //Cho client chon room
            if(loginSuccessful){
                bool joinRoomSuccesful = false;
                while(!joinRoomSuccesful){
                    recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
                    std::string request, username,room;
                    std::string roomAssignFromClient(buffer, recvSize);
                    size_t delimiterPos = roomAssignFromClient.find(',');
                    size_t delimiterPos2 = roomAssignFromClient.rfind(',');
                    if (delimiterPos != std::string::npos || delimiterPos2 != std::string::npos) {
                        request = roomAssignFromClient.substr(0, delimiterPos);
                        username = roomAssignFromClient.substr(delimiterPos + 1, delimiterPos2 - delimiterPos -1);
                        room = roomAssignFromClient.substr(delimiterPos2 + 1);
                    } else {
                        // Neu khong kiem tra duoc dau phay trong chuoi
                        std::cout << "Undefined request from client !" << std::endl;
                        continue;
                    }
                    if(request == "cr00m"){
                        if(checkRoom(room)){
                            send(clientSocket, "603", 3, 0);
                        }
                        else{
                            roomAvailable.insert(room);
                            roomChat(room, clientSocket);
                            saveRoom(room);
                            send(clientSocket, "601", 3, 0);
                            joinRoomSuccesful = true;
                        }
                    }
                    else if(request == "cr00mP"){
                        if(checkRoom(room)){
                            send(clientSocket, "603", 3, 0);
                        }
                        // else{
                        //     roomAvailable.insert(room);
                        //     roomChat(room, clientSocket);
                        //     saveRoom(room);
                        //     send(clientSocket, "601", 3, 0);
                        //     joinRoomSuccesful = true;
                        // }
                        std::string roomKey = generateKey();
                        roomPrivate[roomKey] = room;
                        roomAvailable.insert(room);
                        roomPrivateAllocate.insert(room);
                        roomChat(room, clientSocket);
                        saveRoom(room);
                        std::string prefix = "601,";
                        std::string packageSend = prefix + roomKey;
                        std::cout << packageSend << std::endl;
                        const char* send_m = packageSend.c_str();
                        send(clientSocket, send_m, 10, 0);
                        //send(clientSocket, "601", 3, 0);
                        joinRoomSuccesful = true;
                    }

                    else if(request == "ar00m"){
                        if((roomPrivateAllocate.find(room) == roomPrivateAllocate.end()) && (chatroom.find(room) != chatroom.end())){
                            send(clientSocket, "601", 3, 0);
                            notifyJoin(username,chatroom[room],clientSocket);
                            roomChat(room, clientSocket);
                            joinRoomSuccesful = true;
                        }
                        else if(roomPrivateAllocate.find(room) != roomPrivateAllocate.end()){
                            char bufferVal[1024];
                            send(clientSocket, "666", 3, 0);
                            int checkRec = recv(clientSocket, bufferVal, sizeof(bufferVal), 0);
                            if(checkRec > 0){
                                std::string validate(bufferVal, checkRec);
                                if(roomPrivate.find(validate) != roomPrivate.end()){
                                    notifyJoin(username,chatroom[room],clientSocket);
                                    roomChat(room, clientSocket);
                                    joinRoomSuccesful = true;
                                    send(clientSocket, "601", 3, 0);
                                }
                                else{
                                    send(clientSocket, "602", 3, 0);
                                }
                            }
                        }
                        else{
                            send(clientSocket, "602", 3, 0);
                        }
                    }
                    else{
                        send(clientSocket, "602", 3, 0);
                    }
                }
                break;
            }
        }
    //hjgjh
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        bool join = true;
        // Xử lý phần gửi nhận tin nhắn
        while (true){        
            recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (recvSize > 0) {
                std::string clientMessage(buffer, recvSize);
                std::string historyChat = clientMessage;
                std::cout << "Encrypt: " << historyChat << std::endl;
                historyChat = decryptMessage(historyChat, keyEcrypt);
                Message fullMessage = splitMessage(historyChat);

                std::string extract_username = extractUsername(fullMessage.message);
                //std::cout << "Extracted Username: " << extract_username << std::endl; // Debug

                std::string actual_message = extractActualMessage(fullMessage.message);
                //std::cout << "Actual Message:" << actual_message << std::endl; // Debug

                //ghi lai thong bao chat
                std::ofstream chatHistory(fullMessage.roomName, std::ios::app);
                if(actual_message == " /exit"){
                    std::cout << "Check function when get /exit" << std::endl;
                    std::string notifySaved = "              " + extract_username  +" exit room";
                    chatHistory << notifySaved + '\n'; //chuyen tin nhan thanh chuoi binary va luu vao file
                    chatHistory.close();
                } else {
                    std::cout << "Check function when not get /exit" << std::endl;
                    chatHistory << fullMessage.message + '\n'; 
                    chatHistory.close();
                }

                std::string messageDecrypt = encryptMessage(fullMessage.message, keyEcrypt);
                BroadcastMessage(clientMessage, chatroom[fullMessage.roomName], clientSocket); //gọi hàm gửi tin nhắn cho các client khác
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
    catch (...) {
        std::cerr << "Maybe all user is out or unknown error - Server waiting for connection..." << std::endl;
        if(loggedInAccounts.find(UserName) != loggedInAccounts.end()){
            loggedInAccounts.erase(UserName);
        }
        closesocket(clientSocket); 
        return;
    }
}

std::string generateKey() {
    std::string key;
    srand(time(nullptr));
    const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";
    const size_t maxIndex = sizeof(charset) - 1;
    for (size_t i = 0; i < 6; ++i) {
        key += charset[rand() % maxIndex];
    }
    return key;
}

// Hàm khởi động Servere
int StartServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipServer.c_str()); // Replace with your server IP address
    serverAddr.sin_port = htons(8080); // Replace with your server port number

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    try {
        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed" << std::endl;
                closesocket(serverSocket);
                WSACleanup();
                return 1;
            }
            try {
                std::thread clientThread(HandleClient, clientSocket);
                clientThread.detach();
            } catch (const std::exception &e) {
                std::cerr << "Client handling thread creation failed: " << e.what() << std::endl;
                closesocket(clientSocket);
            } catch (...) {
                std::cerr << "Unknown error occurred while creating client handling thread" << std::endl;
                closesocket(clientSocket);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Server encountered an error: " << e.what() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } catch (...) {
        std::cerr << "Server encountered an unknown error" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

// Hàm main
int main() {
    int choose;
    std::cout<<"Choose Server: " <<std::endl;
    std::cout<<"[1] Server 1 (44)" <<std::endl;
    std::cout<<"[2] Server 2 (41)" <<std::endl;
    std::cout<<"[3] Server 3 (110)" <<std::endl;
    std::cout<<"[4] Server 4 (40)" <<std::endl;
    std::cin >> choose;
    switch(choose) {
        case 1: ipServer = "192.168.37.44";
                break;
        case 2: ipServer = "192.168.37.41";
                break;
        case 3: ipServer = "192.168.37.110";
                break;
        case 4: ipServer = "192.168.37.40";
                break;
        default:
                break;
    }
    StartServer();
}
