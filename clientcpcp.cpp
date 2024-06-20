//   #include "client.h"

// struct client client;
// int endLine = 4;
// std::string senderName;

// // Hàm thực hiện mã hóa
// std::string encryptMessage(const std::string& plaintext, const std::string& key){
//     std::string ciphertext = plaintext; // Khởi tạo chuỗi mã hóa với cùng độ dài như plaintext
    
//     for (size_t i = 0; i < plaintext.size(); ++i) {
//         ciphertext[i] = plaintext[i] ^ key[i % key.size()]; // XOR từng ký tự của plaintext với key
//     }
    
//     return ciphertext; 
// }

// // Hàm thực hiện giải mã
// std::string decryptMessage(const std::string& ciphertext, const std::string& key){
//     std::string decryptedtext = ciphertext; // Khởi tạo chuỗi giải mã với cùng độ dài như ciphertext
    
//     for (size_t i = 0; i < ciphertext.size(); ++i) {
//         decryptedtext[i] = ciphertext[i] ^ key[i % key.size()]; // XOR từng ký tự của ciphertext với key
//     }
    
//     return decryptedtext;
// }   

// // Hàm kiểm tra tính hợp lệ của mật khẩu
// bool isPasswordValid(const std::string& password) {
//     // Sử dụng regex để kiểm tra mật khẩu
//     std::regex pattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]).{8,}");
//     return std::regex_match(password, pattern);
// }

// void GENERATE_LOGIN(){
//     system("cls");
//     setColor(7);
// 	hideCursor(false);
// 	std::string warnLabel = "Welcome user";
// 	std::string continueNoti = "Press 'r' to continue...";

// 	int width = 45;
// 	int height = 10;
// 	int top = 7;
// 	int left = centerWindow(width);
//     std::string checkResponse = "200";
// 	do {
// 		std::string chatLabel = "CHAT APPLICATION";
// 		std::string loginLabel = "LOGIN";

// 		system("cls");
// 		gotoXY(centerWindow(warnLabel.length() - 1), top + 9);
// 		std::cout << warnLabel;
// 		drawRectangle(left, top, width, height);
	
// 		gotoXY(centerWindow(chatLabel.length() - 1), 3);
// 		std::cout << chatLabel;
// 		gotoXY(centerWindow(loginLabel.length() - 1), 5);
// 		std::cout << loginLabel;

// 		gotoXY(left + 5, top + 3);
// 		std::cout << "Username: ";
// 		gotoXY(left + 5, top + 6);
// 		std::cout << "Password: ";

// 		gotoXY(left + 16, top + 3);
// 		std::getline(std::cin>>std::ws, client.username);
//         if(client.username == "/exit"){
//             return FIRST_MENU();
//         }
// 		gotoXY(left + 16, top + 6);
// 		std::getline(std::cin>>std::ws, client.password);

//         std::string message = client.username + "," + client.password + ",1";
        
//         send(clientSocket, message.c_str(), message.length(), 0);
//         char buffer[1024] = {0};
//         int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (recvSize > 0){
//             std::string response(buffer, recvSize);
//             checkResponse = response;
//         } 
//     }while(checkResponse != "201"); //check validate
//     senderName = client.username;
//     return SELECT_ROOM();
// }

// void SELECT_ROOM()
// {
//     int option = optionMenu(8, "CHAT APPLICATION", "WELCOME TO CHAT", 2, "1. New room chat", "2. Available room chat");
//      switch(option){
//         case 1: GENERATE_ROOM(); break;
//         case 2: ENTER_ROOM(); break;
//     }

// }

// void GENERATE_ROOM()
// {
//     std::string roomName = getTextElementBox("Enter room name");
//     std::string roomAssignToServer = "cr00m," + roomName;
//     send(clientSocket, roomAssignToServer.c_str(), roomAssignToServer.length(), 0);
//     char buffer[1024] = {0};
//     int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//     if(recvSize > 0){
//         std::string response(buffer, recvSize);
//         if(response == "603"){
//             notiBox("Room name is already existed");
//             return SELECT_ROOM();
//         }
//         else if(response == "601"){
//             notiBox("Now you can start the conversation");
//             JOIN_CHAT(roomName);
//         }
//     }    
// }

// void ENTER_ROOM()
// {
//     std::string roomName = getTextElementBox("Enter room name");
//     std::string roomAssignToServer = "ar00m," + roomName;
//     send(clientSocket, roomAssignToServer.c_str(), roomAssignToServer.length(), 0);
//     char buffer[1024] = {0};
//     int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//     if(recvSize > 0){
//         std::string response(buffer, recvSize);
//         if(response == "601"){
//             notiBox("Now you can start the conversation");
//             JOIN_CHAT(roomName);
//         }
//     }    
// }

// void GENERATE_SIGNUP(){
//     system("cls");
// 	hideCursor(false);
// 	std::string newPassword = "/";
// 	std::string check;
// 	std::string warnLabel = "Welcome user !";
// 	std::string continueNoti = "Press 'r' to continue...";
// 	int width = 45;
// 	int height = 9;
// 	int top = 7;
// 	int left = centerWindow(width); 
//     bool validPassword = false; // Biến để kiểm tra tính hợp lệ của mật khẩu

// 	while (check != "200") {
//         while(validPassword!=true){
//             std::string chatLabel = "CHAT APPLICATION";
//             std::string loginLabel = "SIGN UP";

//             system("cls");
//             gotoXY(centerWindow(warnLabel.length()), top + 9);
//             std::cout << warnLabel;
//             drawRectangle(left, top, width, height);

//             gotoXY(centerWindow(chatLabel.length()), 3);
//             std::cout << chatLabel;
//             gotoXY(centerWindow(loginLabel.length()), 5);
//             std::cout << loginLabel;

//             gotoXY(left + 5, top + 2);
//             std::cout << "Username: ";
//             gotoXY(left + 5, top + 4);
//             std::cout << "Password: ";
//             gotoXY(left + 5, top + 6);
//             std::cout << "Confirm Password: ";

//             gotoXY(left + 16, top + 2);
//             std::cin >> client.username;
//             if(client.username == "/exit"){
//                 return FIRST_MENU();
//             }
//             gotoXY(left + 16, top + 4);
//             std::cin >> client.password;
//             gotoXY(left + 23, top + 6);
//             std::cin >> newPassword;

//             if(client.username == "/exit"){
//                 return FIRST_MENU();
//             }

//             if(client.password != newPassword){
//                 warnLabel = "Password does not match";
//                 continue;
//             }
//             if (isPasswordValid(newPassword)) { // Kiểm tra mật khẩu hợp lệ
//                 validPassword = true;
//             } else {
//                 //warnLabel = "Password is invalid. Please try again.";
//                 validPassword = true; // Thông báo lỗi nếu mật khẩu không hợp lệ
//             }
//         }

//         std::string message = client.username + "," + client.password + ",0";
//         send(clientSocket, message.c_str(), message.length(), 0);
//         char buffer[1024] = {0};
//         int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (recvSize > 0) {
//             std::string response(buffer, recvSize);
//             check = response;
//             if (response == "200") {
//                 notiBox("Sign up completed");
//             } 
//             else if(check == "403"){
//                 warnLabel = "Username is already existed";
//                 continue;
//             }
//             else {
//                 notiBox("Something wrong with server");
//             }
//         }
// 	}
// 	//global.employee.setEmployeePassword(newPassword);
//     //Set password and login name for user here
//     // std::string message = client.username + "," + client.password + ",0";
//     // send(clientSocket, message.c_str(), message.length(), 0);
// 	// system("cls");
	
//     // char buffer[1024] = {0};
//     // int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//     // if (recvSize > 0) {
//     //     std::string response(buffer, recvSize);
//     //     if (response == "200") {
//     //         notiBox("Sign up completed");
//     //     } 
//     //     else if(response == "403"){
//     //         notiBox("Username is already existed");
//     //     }
//     //     else {
//     //         notiBox("Something wrong with server");
//     //     }
//     // }
//     return GENERATE_LOGIN();
// }

// void receiveMessages(SOCKET clientSocket) {
//     char buffer[4096]; // Tăng kích thước buffer lên 4096 byte
//     std::string messageAfter;
//     std::string label = "====================== ROOM CHAT 1 ======================";
//     while (true) {
//         int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
//         gotoXY(centerWindow(label.length()) + 4, ++endLine);
//         if (recvSize > 0) {
//             std::string message(buffer, recvSize);
//             messageAfter = decryptMessage(message, keyEncrypt);
//             std::cout << messageAfter << std::endl;
//         }
//         gotoXY(22, 2);
//     }
// }


// void FIRST_MENU()
// {
//     int option = optionMenu(8, "CHAT APPLICATION", "LOGIN OR SIGN UP", 2, "1. Login", "2. Sign up");
//     switch(option){
//         case 1: GENERATE_LOGIN(); break;
//         case 2: GENERATE_SIGNUP(); break;
//     }
// }

// int SOCKET_START(SOCKET &clientSocket)
// {
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         notiBox("WSAStartup failed");
//         return 1;
//     }

//     clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if (clientSocket == INVALID_SOCKET) {
//         notiBox("Socket creation failed");
//         WSACleanup();
//         return 1;
//     }

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(PORT);
//     serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);

//     if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//         notiBox("Connection failed");
//         closesocket(clientSocket);
//         WSACleanup();
//         return 1;
//     }
//     return 0;
// }

// void SET_UP_CONNECTION(){
//     int socketComplete;
//     do{
//         

//         loadingEffect("Connecting to server...");
//         socketComplete = SOCKET_START(clientSocket);
//     } while(socketComplete);
// }

// void RESET_SOCKET()
// {
//     closesocket(clientSocket); // Close the existing socket
//     clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create a new socket
// }

// void IP_CONFIG()
// {
//     int IP_GENERATE = optionMenu(9, "CHAT APPLICATION", "IP SERVER", 4, "192.168.37.110", "192.168.37.44", "192.168.37.40", "192.168.37.41");
//     switch(IP_GENERATE){
//         case 1: IP_SERVER = "192.168.37.110"; break;
//         case 2: IP_SERVER = "192.168.37.44"; break;
//         case 3: IP_SERVER = "192.168.37.40"; break;
//         case 4: IP_SERVER = "192.168.37.41"; break;
//     }
//     system("cls");
// }

// void JOIN_CHAT(std::string roomName)
// {
//     system("cls");
//     hideCursor(true);
//     std::string messageBefore;
//     int width = 80;
// 	int height = 1;
// 	int top = 1;
// 	int left = centerWindow(width);
//     char key;
//     std::string label_s = "======================";
//     std::string label = label_s + " " + roomName + " " + label_s;
//     std::thread receiveThread(receiveMessages, clientSocket);
//     receiveThread.detach();
//     gotoXY(centerBox(width, label.length()), 4);
//     std::cout << label;
//     std::string msg = "";
//     while (msg != "/exit") {
//         key = _getch();
//         drawRectangle(left, top, width, height);
//         if(key == 't' || key == 'T'){
//             hideCursor(false);
//             gotoXY(22, 2);
//             std::cout << "You: ";
//             std::getline(std::cin >>std::ws, msg);
            
//             gotoXY(22, 2); // Move cursor to the beginning of the input line
//             for (size_t i = 0; i < msg.length() + 5; ++i) { // +5 to cover "You: " prefix
//                 std::cout << " "; // Overwrite with spaces
//             }
//             //Move cursor back to the beginning of the input line
//             gotoXY(27, 2);
//             gotoXY(centerWindow(label.length()), ++endLine);
//             std::cout << "You: " << msg;
//             std::string fullMessage = senderName + ": " + msg;          // Tạo tin nhắn đầy đủ
//             messageBefore = encryptMessage(fullMessage, keyEncrypt);
//             send(clientSocket, messageBefore.c_str(), messageBefore.length(), 0);
//         }
//         removeRectangle(left, top, width, height);
//     }
//     return SELECT_ROOM();
// }

// int main(){
//     WINDOW_RESOLUTION();
//     SET_UP_CONNECTION();
//     FIRST_MENU();
// }

#include "client.h"

struct client client;
int endLine = 4;
std::string senderName;

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

std::string createLabel(const std::string &roomName, size_t size, char fillChar)
{
    size_t fillSize = (roomName.size() < size) ? (size - roomName.size()) / 2 : 0;

    std::string fill(fillSize, fillChar);

    std::string label = fill + " " + roomName + " " + fill;
    if (label.size() < size) {
        label += fillChar;
    }

    return label;
}

std::string labelUser(const std::string &line, const std::string &username)
{
    size_t pos = line.find(":");
    if (pos != std::string::npos) {
        std::string name = line.substr(0, pos);
        if (name == username) {
            return "You" + line.substr(pos);
        }
    }
    return line;
}

void printMessage(std::string &message)
{
    int terminalWidth = getTerminalWidth();
    int currentLineWidth = 0;
    std::string label = "=================================================";
    for (char c : message) {
        if (c == '\n') {
            std::cout << c;
            currentLineWidth = 0;
        } else {
            std::cout << c;
            currentLineWidth++;

            if (currentLineWidth >= terminalWidth/2.5) {
                gotoXY(centerWindow(label.length()+1), ++endLine);
                currentLineWidth = 0;
            }
        }
    }
}

std::string inputPassword(int x, int y) {
    std::string password;
    bool showPassword = true;
    char ch;

    while (true) {
        ch = _getch();
        if (ch == 13) { // Enter key
            break;
        } else if (ch == 9) { // Tab key
            showPassword = !showPassword;
            gotoXY(x, y);
            if (showPassword) {
                std::cout << password;
            } else {
                std::cout << std::string(password.length(), '*');
            }
            std::cout.flush();
        } else if (ch == 8) { // Backspace key
            if (!password.empty()) {
                password.pop_back();
                gotoXY(x + password.length(), y);
                std::cout << " \b";
            }
        } else {
            password += ch;
            if (showPassword) {
                std::cout << ch;
            } else {
                std::cout << '*';
            }
        }
    }
    return password;
}

void GENERATE_LOGIN()
{
    system("cls");
    setColor(7);
	hideCursor(false);
	std::string warnLabel = "Welcome user";
	std::string continueNoti = "Press 'r' to continue...";

	int width = 45;
	int height = 10;
	int top = 7;
	int left = centerWindow(width);
    std::string checkResponse = "200";
	do {
		std::string chatLabel = "CHAT APPLICATION";
		std::string loginLabel = "LOGIN";

		system("cls");
		gotoXY(centerWindow(warnLabel.length() - 1), top + 9);
		std::cout << warnLabel;
		drawRectangle(left, top, width, height);
	
		gotoXY(centerWindow(chatLabel.length() - 1), 3);
		std::cout << chatLabel;
		gotoXY(centerWindow(loginLabel.length() - 1), 5);
		std::cout << loginLabel;

		gotoXY(left + 5, top + 3);
		std::cout << "Username: ";
		gotoXY(left + 5, top + 6);
		std::cout << "Password: ";
        gotoXY(left + 40, top + 6);
        std::cout << "[TAB]" << std::endl;
		gotoXY(left + 16, top + 3);
		std::getline(std::cin>>std::ws, client.username);
        if(client.username == "/exit"){
            return FIRST_MENU();
        }
		gotoXY(left + 16, top + 6);
		// std::getline(std::cin>>std::ws, client.password);
        client.password = inputPassword(left + 16, top + 6);
        std::string message = client.username + "," + client.password + ",1";
        
        send(clientSocket, message.c_str(), message.length(), 0);
        char buffer[1024] = {0};
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvSize > 0){
            std::string response(buffer, recvSize);
            checkResponse = response;
        } 
    }while(checkResponse != "201"); //check validate
    senderName = client.username;
    return SELECT_ROOM();
}

void SELECT_ROOM()
{
    int option = optionMenu(8, "CHAT APPLICATION", "WELCOME TO CHAT", 2, "1. New room chat", "2. Available room chat");
    switch(option){
        case 1: GENERATE_ROOM(); break;
        case 2: ENTER_ROOM(); break;
    }

}



void GENERATE_ROOM()
{
    int option = optionMenu(8, "CHAT APPLICATION", "CHOOSE ROOM CATEGORIZE", 2, "1. Public room (Everyone can join)", "2. Private room (Join with key)");
    switch(option){
        case 1: PUBLIC_ROOM_CREATE(); break;
        case 2: PRIVATE_ROOM_CREATE(); break;
        case 3: return SELECT_ROOM(); break;
    }
}

void ENTER_ROOM()
{
    std::string roomName = SHOW_AVAILABLE_ROOM("C://Users//intern.hhtu1//Desktop//Zalo_Rep//server//Room.txt");
    std::string roomAssignToServer = "ar00m," + client.username + "," + roomName;
    send(clientSocket, roomAssignToServer.c_str(), roomAssignToServer.length(), 0);
    char buffer[1024] = {0};
    int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(recvSize > 0){
        std::string response(buffer, recvSize);
        if(response == "601"){
            notiBox("Now you can start the conversation");
            REQUEST_TO_SERVER(chatDatabase, roomName);
            JOIN_CHAT(roomName);
        }
        else if(response == "666"){
            std::string userValidateRoomKey = getTextElementBox("ENTER KEY ROOM");
            send(clientSocket, userValidateRoomKey.c_str(), userValidateRoomKey.length(), 0);
            char bufferP[1024] = {0};
            int recvSizeP = recv(clientSocket, bufferP, sizeof(bufferP), 0);
            if(recvSizeP > 0){
                std::string response(bufferP, recvSizeP);
                if(response == "601"){
                    notiBox("Now you can start the conversation");
                    REQUEST_TO_SERVER(chatDatabase, roomName);
                    JOIN_CHAT(roomName);
                }
                else{
                    notiBox("Invalid Key");
                    return SELECT_ROOM();
                }
            }
            // if(userValidateRoomKey == key){
            //     notiBox("Now you can start the conversation");
            //     REQUEST_TO_SERVER(chatDatabase, roomName);
            //     JOIN_CHAT(roomName);
            // }
            // else{
            //     notiBox("Invalid Key");
            //     return SELECT_ROOM();
            // }
        }
        else{
            notiBox("Cannot join room due to specific error");
            return SELECT_ROOM();
        }
    }    
}

void GENERATE_SIGNUP(){
    system("cls");
	hideCursor(false);
	std::string newPassword = "/";
	std::string check;
	std::string warnLabel = "Welcome user !";
	std::string continueNoti = "Press 'r' to continue...";
	int width = 45;
	int height = 9;
	int top = 7;
	int left = centerWindow(width);
	while (client.password != newPassword || check != "200") {
		std::string chatLabel = "CHAT APPLICATION";
		std::string loginLabel = "SIGN UP";

		system("cls");
		gotoXY(centerWindow(warnLabel.length()), top + 9);
		std::cout << warnLabel;
		drawRectangle(left, top, width, height);

		gotoXY(centerWindow(chatLabel.length()), 3);
		std::cout << chatLabel;
		gotoXY(centerWindow(loginLabel.length()), 5);
		std::cout << loginLabel;

		gotoXY(left + 5, top + 2);
		std::cout << "Username: ";
		gotoXY(left + 5, top + 4);
		std::cout << "Password: ";
		gotoXY(left + 5, top + 6);
		std::cout << "Confirm Password: ";
        gotoXY(left + 40, top + 6);
        std::cout << "[TAB]" << std::endl;
		gotoXY(left + 16, top + 2);
		std::cin >> client.username;
        if(client.username == "/exit"){
            return FIRST_MENU();
        }
        gotoXY(left + 16, top + 4);
        client.password = inputPassword(left + 16, top + 4);

        gotoXY(left + 23, top + 6);
        newPassword = inputPassword(left + 23, top + 6);
        if(client.password != newPassword){
            warnLabel = "Password does not match";
            continue;
        }

        std::string message = client.username + "," + client.password + ",0";
        send(clientSocket, message.c_str(), message.length(), 0);
        char buffer[1024] = {0};
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvSize > 0) {
            std::string response(buffer, recvSize);
            check = response;
            if (response == "200") {
                notiBox("Sign up completed");
            } 
            else if(check == "403"){
                warnLabel = "Username is already existed";
                continue;
            }
            else if(check == "507")
            {
                warnLabel = "ERROR";
                continue;
            }
            else {
                notiBox("Something wrong with server");
            }
        }
	}
	//global.employee.setEmployeePassword(newPassword);
    //Set password and login name for user here
    // std::string message = client.username + "," + client.password + ",0";
    // send(clientSocket, message.c_str(), message.length(), 0);
	// system("cls");
	
    // char buffer[1024] = {0};
    // int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    // if (recvSize > 0) {
    //     std::string response(buffer, recvSize);
    //     if (response == "200") {
    //         notiBox("Sign up completed");
    //     } 
    //     else if(response == "403"){
    //         notiBox("Username is already existed");
    //     }
    //     else {
    //         notiBox("Something wrong with server");
    //     }
    // }
    return GENERATE_LOGIN();
}

void IP_CONFIG()
{
    int IP_GENERATE = optionMenu(9, "CHAT APPLICATION", "IP SERVER", 4, "192.168.37.110", "192.168.37.44", "192.168.37.40", "192.168.37.41");
    switch(IP_GENERATE){
        case 1: IP_SERVER = "192.168.37.110"; break;
        case 2: IP_SERVER = "192.168.37.44"; break;
        case 3: IP_SERVER = "192.168.37.40"; break;
        case 4: IP_SERVER = "192.168.37.41"; break;
    }
    system("cls");
}

void LOAD_HISTORY_CHAT(std::string &chatDatabase, std::string &username)
{
    std::ifstream file(chatDatabase);
    // if (!file.is_open()) {
    //     std::cerr << "Failed to load history chat! " << chatDatabase << std::endl;
    //     return;
    // }

    std::string line;
    const int fixedWidth = 50;
    std::string label = std::string((fixedWidth - username.length() - 2) / 2, '=') 
                        + " " + username + " " 
                        + std::string((fixedWidth - username.length() - 2) / 2, '=');

    while (std::getline(file, line)) {
        line = labelUser(line, username);
        gotoXY(centerWindow(label.length()) - 1, ++endLine);
        std::cout << line << std::endl;
    }

    file.close();
}

void REQUEST_TO_SERVER(std::string &chatDatabase, std::string roomName)
{
    chatDatabase = "C://Users//intern.hhtu1//Desktop//Zalo_Rep//server//" + roomName;
}

std::vector<std::string> readDatabaseFile(const std::string& fileName) {
    std::vector<std::string> roomDatabase;
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return roomDatabase;
    }

    std::string line;
    while (std::getline(file, line)) {
        roomDatabase.push_back(line);
    }

    file.close();
    return roomDatabase;
}

std::string SHOW_AVAILABLE_ROOM(const std::string &chatDatabase)
{
    system("cls");
    std::vector<std::string> roomDatabase = readDatabaseFile(chatDatabase);
    std::string returnRoom;
    std::string chatLabel = "PICK YOUR ROOM AND HAVE FUN";
    int counter = 1, width = 30, height = roomDatabase.size() + 7, top = 7, lefts = centerWindow(width), leftBox = centerBox(width, 3), geti;
    char key;
    int pageCounter = roomDatabase.size();
    std::vector<int> initMenuColor;
    for(int i = 0; i < roomDatabase.size() + 3; i++){
        initMenuColor.push_back(7);
    }
    while(true){
        if(roomDatabase.empty()){
            notiBox("No room now available now");
            SELECT_ROOM();
        }
        else{
            setColor(7);
            gotoXY(centerWindow(chatLabel.length() - 1), 3);
            std::cout << chatLabel;
            drawRectangle(lefts, top, width, height);
            for(int i = 0; i < roomDatabase.size(); i++){
                gotoXY(leftBox, top + 2 + i + 1);
                setColor(initMenuColor[i]);
                std::cout << roomDatabase[i]; 
                geti = i;
            }
            gotoXY(centerWindow(3), top + 5 + geti + 1);
            setColor(initMenuColor[geti + 1]);
            std::cout << "Exit";
            // std::cout << counter << std::endl;
            // std::cout << pageCounter << std::endl;
            key = _getch();
            if(key == 'w' && counter >= 2 && counter <= roomDatabase.size() + 3){counter--;}
            if(key == 's' && counter >= 1 && counter <= roomDatabase.size() + 2){counter++;}
            if(key == '\r'){
                if(counter == pageCounter + 1){
                    SELECT_ROOM();
                }
                else{
                    returnRoom = roomDatabase[counter - 1];
                    return returnRoom;
                }
            }
            for(int i = 0; i < roomDatabase.size() + 3; i++){
                initMenuColor[i] = 7;
            }
            initMenuColor[counter - 1] = 3;
        }
    }
}

void receiveMessages(SOCKET clientSocket)
{
    char buffer[4096]; // Tăng kích thước buffer lên 4096 byte
    std::string messageAfter;
    std::string label = "=================================================";
    while (true) {
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        gotoXY(centerWindow(label.length()) - 1, ++endLine);
        if (recvSize > 0) {
            std::string message(buffer, recvSize);
            messageAfter = decryptMessage(message, keyEncrypt);
            std::cout << messageAfter << std::endl;
        }
        gotoXY(22, 2);
    }
}

void PUBLIC_ROOM_CREATE()
{
    std::string roomName = getTextElementBox("Enter room name");
    std::string roomAssignToServer = "cr00m," + roomName;
    send(clientSocket, roomAssignToServer.c_str(), roomAssignToServer.length(), 0);
    char buffer[1024] = {0};
    int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(recvSize > 0){
        std::string response(buffer, recvSize);
        if(response == "603"){
            notiBox("Room name is already existed");
            return SELECT_ROOM();
        }
        else if(response == "601"){
            notiBox("Now you can start the conversation");
            REQUEST_TO_SERVER(chatDatabase, roomName);
            JOIN_CHAT(roomName);
        }
    }
}

void PRIVATE_ROOM_CREATE()
{
    std::string roomName = getTextElementBox("Enter room name");
    std::string roomAssignToServer = "cr00mP," + roomName;
    send(clientSocket, roomAssignToServer.c_str(), roomAssignToServer.length(), 0);
    char buffer[1024] = {0};
    int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(recvSize > 0){
        std::string response(buffer, recvSize);
        if(response == "603"){
            notiBox("Room name is already existed");
            return SELECT_ROOM();
        }
        else if(response == "602"){
            notiBox("Cannot join room due to specific error");
            return SELECT_ROOM();
        }
        else{
            size_t commaPos = response.find(',');
            std::string prefix = response.substr(0, commaPos);
            keyRoom = response.substr(commaPos + 1);
            notiBox("YOUR ROOM KEY IS: " + keyRoom);
            //std::string userValidateRoomKey = getTextElementBox("ENTER KEY ROOM");
            //if(userValidateRoomKey == key){
            notiBox("Now you can start the conversation");
            REQUEST_TO_SERVER(chatDatabase, roomName);
            JOIN_CHAT(roomName);
            // }
            // else{
            //     notiBox("Invalid Key");
            //     return SELECT_ROOM();
            // }
        }
    }
}

void FIRST_MENU()
{
    int option = optionMenu(8, "CHAT APPLICATION", "LOGIN OR SIGN UP", 2, "1. Login", "2. Sign up");
    switch(option){
        case 1: GENERATE_LOGIN(); break;
        case 2: GENERATE_SIGNUP(); break;
    }
}

int SOCKET_START(SOCKET &clientSocket)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        notiBox("WSAStartup failed");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        notiBox("Socket creation failed");
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        notiBox("Connection failed");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

void SET_UP_CONNECTION(){
    int socketComplete;
    do{
        IP_CONFIG();
        loadingEffect("Connecting to server...");
        socketComplete = SOCKET_START(clientSocket);
    } while(socketComplete);
}

void RESET_SOCKET()
{
    closesocket(clientSocket); // Close the existing socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create a new socket
}

void JOIN_CHAT(std::string roomName)
{
    system("cls");
    hideCursor(true);
    std::string messageBefore;
    int width = 80;
	int height = 1;
	int top = 1;
	int left = centerWindow(width);
    char key;
    std::string label_s = "======================";
    std::string label = createLabel(roomName);
    std::thread receiveThread(receiveMessages, clientSocket);
    receiveThread.detach();
    gotoXY(centerBox(width, label.length()), 4);
    std::cout << label;
    std::string msg = "";
    gotoXY(22, 2);
    LOAD_HISTORY_CHAT(chatDatabase, client.username);
    while (msg != "/exit") {
        key = _getch();
        drawRectangle(left, top, width, height);
        if(key == 't' || key == 'T'){
            hideCursor(false);
            gotoXY(23, 2);
            std::cout << "You: ";
            std::getline(std::cin >>std::ws, msg);
            if(msg == "/smile") msg = "(^_^)";
            else if(msg == "/sad") msg = "(T_T)";
            else if(msg == "/wow") msg = "(O.O)";
            else if(msg == "/scare") msg = "(|-.-|)";
            else if(msg == "/key") msg = keyRoom;
            gotoXY(23, 2); // Move cursor to the beginning of the input line
            for (size_t i = 0; i < msg.length() + 5; ++i) { // +5 to cover "You: " prefix
                std::cout << " "; // Overwrite with spaces
            }
            //Move cursor back to the beginning of the input line
            gotoXY(27, 2);
            gotoXY(centerWindow(label.length()), ++endLine);
            std::cout << "You: ";
            printMessage(msg);
            std::string fullMessage = roomName + "," + senderName + ": " + msg;          // Tạo tin nhắn đầy đủ
            messageBefore = encryptMessage(fullMessage, keyEncrypt);
            send(clientSocket, messageBefore.c_str(), messageBefore.length(), 0);
        }
        removeRectangle(left, top, width, height);
    }
    return SELECT_ROOM();
}

int main(){
    WINDOW_RESOLUTION();
    SET_UP_CONNECTION();
    FIRST_MENU();
}
