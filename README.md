# Chat Server with Encryption and Account Management

This project is a multi-client chat server implemented in C++ with encryption and account management features. It supports account creation, login/logout functionality, and encrypted message broadcasting to multiple clients. The server uses the Winsock API for network communication and multithreading for handling multiple clients concurrently.

## Features

- **Multi-client support**: Handles multiple clients simultaneously using threads.
- **Account management**: Supports account creation, login, and logout.
- **Message encryption**: Messages are encrypted using a simple XOR-based encryption.
- **Broadcast messages**: Messages from one client are broadcast to all other connected clients.
- **Concurrency control**: Uses mutexes to ensure thread-safe access to shared resources.

## Directory Structure

- ├── server.cpp
- ├── server.h
- ├── server.exe
- ├── account.txt
- └── README.md

## Getting Started

### Prerequisites

- Windows OS (for Winsock API)
- Code editor (VSCode)
- C++ compiler (e.g., MSVC, MinGW)
- CMake (for building the project)

### Installation

1. **Clone the repository**:

    ```sh
    git clone https://github.com/hyutrn/Chat-Application.git
    cd server
    ```

2. **Build the project**:

    ```sh
    g++ -g server.cpp -o server -lws2_32
    ```

3. **Run the server**:

    ```sh
    ./server.exe
    ```

### Connecting Clients

Clients can connect to the server using a custom client application on client branch:

```sh
./client.exe
```
## Code Overview

### main.cpp
Contains the main function and the server startup code. It initializes the server, listens for incoming client connections, and spawns a new thread to handle each client.

### server.h
Contains the declarations for server functions and necessary includes. It also defines the `Account` struct used for account management.

### Account Management
- **Account Creation**: When a client sends a message with a key value of 0, the server creates a new account with a unique ID.
- **Login**: Clients can log in with their username and password. The server checks if the account exists and is not already logged in.
- **Logout**: When a client logs out, the server updates the account's status.

### Message Handling
- **Encryption**: Messages are encrypted and decrypted using a simple XOR operation with a key.
- **Broadcast**: Messages from one client are sent to all other connected clients in the chat room.

### Concurrency Control
Mutexes are used to ensure that access to shared resources (like the list of clients and account states) is thread-safe.

## Example Usage

### Create an account:
- **Client sends**: `username,password,0`
- **Server response**: `200` (success) or `403` (username exists)

### Login:
- **Client sends**: `username,password,1`
- **Server response**: `201` (success) or `401` (login failed)

### Send a message:
- **Client sends**: `message`
- **Server broadcasts the message to all other clients.

### Logout:
- **Client sends**: `./exit`
- **Server notifies other clients that the user has left the chat room.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


