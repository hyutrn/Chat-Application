# Chat Application

## Notice
- The program only work with server and client in this branch. Do not use other client or server from other branch.
- Run the program following the instruction below.

## New feature update (18/06)
- Store and load history chat for each room.
- Multi-room chat has been available.
- New command for emoji.
- Repair choose server IP before connect.
- Handling out of range message (auto endline when message is too long).
- /exit command to exit current page.
Notice: Change the directory in client.cpp -> REQUEST_TO_SERVER() function to current directory of SERVER to get the chat history.

## New feature update (14/06)
- New function: Choose server IP before connect.

## New feature update (11/06)
- Room handle from server.
- Add new check code from server to client:
  + 601: Succesful - Allow client to join the room.
  + 602: Error - Cannot join the room.
  + 603: Create a room which is existed - Cannot create room
- New request command from client to server:
  + "cr00m": Request server create a new room chat.
  + "ar00m": Request server to join in an available room chat.
- Update more GUI function for client.

## New feature update (07/06)
- Add room selection.
- Store chat history.
- GUI function update.
- GUI interface update.
- Temporary server for tesing client.

## Overview

This chat application allows multiple clients to communicate with each other through a server. It supports user registration, login, encrypted messaging, and real-time message broadcasting.

## File Structure

- client
- ├── GUI.cpp
- ├── GUI.h
- ├── client.cpp
- ├── client.h
- ├── main.exe
- ├── account.txt
- └── README.md

### File Descriptions

#### GUI.cpp
- Contains functions to handle the graphical user interface (GUI) components such as drawing rectangles, setting colors, and handling cursor visibility. 

#### GUI.h
- Header file for `GUI.cpp`, includes declarations for GUI functions.

#### client.cpp
- Implements the client-side logic of the chat application, including account management, message handling, and server communication.

#### client.h
- Header file for `client.cpp`, includes declarations for client-related functions and the `client` struct.

#### main.exe
- The compiled executable file of the chat application.

#### account.txt
- Stores account information, such as usernames and passwords.

#### README.md
- This file, containing the description and usage information of the chat application.

## Features

### Account Management
- **Account Creation**: Clients can create a new account with a unique username.
- **Login**: Clients can log in with their username and password.
- **Logout**: Clients can log out, notifying other clients.

### Message Handling
- **Encryption**: Messages are encrypted using a simple XOR operation before being sent to the server.
- **Broadcast**: Messages from one client are broadcast to all other connected clients.

### Concurrency Control
- **Thread Safety**: Mutexes are used to ensure thread-safe access to shared resources, such as the list of connected clients and account information.

## Example Usage

### Create an Account
1. Client sends: `username,password,0`
2. Server response: `200` (success) or `403` (username exists)

### Login
1. Client sends: `username,password,1`
2. Server response: `201` (success) or `401` (login failed)

### Send a Message
1. Client sends: `message`
2. Server broadcasts the message to all other clients.

### Logout
1. Client sends: `./exit`
2. Server notifies other clients that the user has left the chat room.

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
    cd client
    ```

2. **Build the project**:

    ```sh
    g++ -g client.cpp GUI.cpp -o main -lws2_32
    ```
    or
    ```sh
    ./compile.bat
    ```

### Connecting Clients to Server

Clients can connect to the server using a custom client application on client branch, open cmd in folder project and run this command:

```sh
./main.exe
```

## Notice: Running test Server (before running client)
1. **Build server**:
    ```sh
    g++ -g server.cpp -o server -lws2_32
    ```
2. **Run server**:
    
    ```sh
    ./server.exe
    ```


