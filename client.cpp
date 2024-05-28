#include "client.h"

struct client client;
int endLine = 4;

void GENERATE_LOGIN(){
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
    notiBox("Now you can start the conversation");
    return JOIN_CHAT();
}

void GENERATE_SIGNUP(){
    system("cls");
	hideCursor(false);
	std::string newPassword = "/";
	std::string username;
	std::string warnLabel = "Welcome user !";
	std::string continueNoti = "Press 'r' to continue...";
	int width = 45;
	int height = 9;
	int top = 7;
	int left = centerWindow(width);
	while (client.password != newPassword) {
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

		warnLabel = "Password does not match";
	}
	//global.employee.setEmployeePassword(newPassword);
    //Set password and login name for user here
    std::string message = client.username + "," + client.password + ",0";
    send(clientSocket, message.c_str(), message.length(), 0);
	system("cls");
	
    char buffer[1024] = {0};
    int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvSize > 0) {
        std::string response(buffer, recvSize);
        if (response == "200") {
            notiBox("Sign up completed");
        } else {
            notiBox("Something wrong with server");
        }
    }
    return GENERATE_LOGIN();
}

void receiveMessages(SOCKET clientSocket) {
    char buffer[4096]; // Tăng kích thước buffer lên 4096 byte
    std::string label = "============ ROOM CHAT 1 ============";
    while (true) {
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        gotoXY(centerWindow(label.length()), ++endLine);
        if (recvSize > 0) {
            std::string message(buffer, recvSize);
            std::cout << message << std::endl;
        }
        gotoXY(22, 26);
        std::cout << "You: ";
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

void JOIN_CHAT()
{
    int width = 80;
	int height = 1;
	int top = 25;
	int left = centerWindow(width);
    drawRectangle(left, top, width, height);
    std::string label = "============ ROOM CHAT 1 ============";
    std::thread receiveThread(receiveMessages, clientSocket);
    receiveThread.detach();
    gotoXY(centerBox(width, label.length()), 4);
    std::cout << label;
    while (true) {
        gotoXY(22, 26);
        std::string msg;
        std::cout << "You: ";
        std::getline(std::cin >>std::ws, msg);
        gotoXY(27, 26); // Move cursor to the beginning of the input line
        for (size_t i = 0; i < msg.length() + 5; ++i) { // +5 to cover "You: " prefix
            std::cout << " "; // Overwrite with spaces
        }

        // Move cursor back to the beginning of the input line
        gotoXY(27, 26);
        gotoXY(centerWindow(label.length()), ++endLine);
        std::cout << "You: " << msg;
        send(clientSocket, msg.c_str(), msg.length(), 0);
    }
}

int main(){
    SET_UP_CONNECTION();
    FIRST_MENU();
}
