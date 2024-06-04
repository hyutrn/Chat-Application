# Chat Application

This project is a multi-client chat application implemented in C++ with encryption and account management features. It consists of two main components: a server and a client. The server manages client connections, handles account management, and facilitates message broadcasting, while the client provides a graphical user interface (GUI) for users to interact with the chat system.

## Features

- **Multi-client Support**: Handles multiple clients simultaneously using threads.
- **Account Management**: Supports account creation, login, and logout functionalities.
- **Message Encryption**: Encrypts messages using a simple XOR-based encryption method.
- **Broadcast Messages**: Broadcasts messages from one client to all other connected clients.
- **Concurrency Control**: Ensures thread-safe access to shared resources using mutexes.

## Directory Structure

- Chat-Application/
- ├── server/
- │ ├── server.cpp
- │ ├── server.h
- │ ├── account.txt
- │ └── README.md
- └── client/
- ├── GUI.cpp
- ├── GUI.h
- ├── client.cpp
- ├── client.h
- ├── compile.bat
- └── README.md


## Server

The server component manages client connections and handles account management and message broadcasting.

### File Structure

- `server.cpp`: Contains the main server implementation.
- `server.h`: Header file containing declarations for server functions.
- `account.txt`: Stores account information.
- `README.md`: Description and usage information for the server.

## Client

The client component provides a GUI for users to interact with the chat system, including account management and message handling.

### File Structure

- `GUI.cpp`: Functions to handle the GUI components.
- `GUI.h`: Header file containing declarations for GUI functions.
- `client.cpp`: Client-side logic implementation.
- `client.h`: Header file containing declarations for client functions.
- `compile.bat`: Batch file for compiling the client.
- `account.txt`: Stores account information.
- `README.md`: Description and usage information for the client.

## Getting Started

### Prerequisites

- Windows OS (for Winsock API).
- Code editor (e.g., VSCode).
- C++ compiler (e.g., MSVC, MinGW).
- CMake (for building the project).

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/hyutrn/Chat-Application.git
    ```

2. Navigate to the `server` or `client` directory, depending on which component you want to run.
3. Build the project using the provided instructions in the respective README.md file.
4. Run the server and client applications following the instructions in their respective README.md files.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
