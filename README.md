# Chat Application

## Overview

This chat application allows multiple clients to communicate with each other through a server. It supports user registration, login, encrypted messaging, and real-time message broadcasting.

## File Structure

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

## Function Descriptions

### Encryption and Decryption
```cpp
std::string encryptMessage(const std::string& plaintext, const std::string& key);
std::string decryptMessage(const std::string& ciphertext, const std::string& key);
```
These functions perform encryption and decryption of messages using XOR operation with a given key.

# GUI Functions

Functions to manage the graphical user interface, such as drawing elements on the screen and setting console properties.


