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

void GENERATE_LOGIN(){
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

		gotoXY(left + 16, top + 3);
		std::getline(std::cin>>std::ws, client.username);
		gotoXY(left + 16, top + 6);
		std::getline(std::cin>>std::ws, client.password);
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
    }

}

void GENERATE_ROOM()
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
            JOIN_CHAT(roomName);
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

		gotoXY(left + 16, top + 2);
		std::cin >> client.username;
		gotoXY(left + 16, top + 4);
		std::cin >> client.password;
		gotoXY(left + 23, top + 6);
		std::cin >> newPassword;

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

void receiveMessages(SOCKET clientSocket) {
    char buffer[4096]; // Tăng kích thước buffer lên 4096 byte
    std::string messageAfter;
    std::string label = "====================== ROOM CHAT 1 ======================";
    while (true) {
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        gotoXY(centerWindow(label.length()) + 4, ++endLine);
        if (recvSize > 0) {
            std::string message(buffer, recvSize);
            messageAfter = decryptMessage(message, keyEncrypt);
            std::cout << messageAfter << std::endl;
        }
        gotoXY(22, 2);
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
    std::string label = label_s + " " + roomName + " " + label_s;
    std::thread receiveThread(receiveMessages, clientSocket);
    receiveThread.detach();
    gotoXY(centerBox(width, label.length()), 4);
    std::cout << label;
    std::string msg = "";
    while (msg != "/exit") {
        key = _getch();
        drawRectangle(left, top, width, height);
        if(key == 't' || key == 'T'){
            hideCursor(false);
            gotoXY(22, 2);
            std::cout << "You: ";
            std::getline(std::cin >>std::ws, msg);
            
            gotoXY(22, 2); // Move cursor to the beginning of the input line
            for (size_t i = 0; i < msg.length() + 5; ++i) { // +5 to cover "You: " prefix
                std::cout << " "; // Overwrite with spaces
            }
            //Move cursor back to the beginning of the input line
            gotoXY(27, 2);
            gotoXY(centerWindow(label.length()), ++endLine);
            std::cout << "You: " << msg;
            std::string fullMessage = senderName + ": " + msg;          // Tạo tin nhắn đầy đủ
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
