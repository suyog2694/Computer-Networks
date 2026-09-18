#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <winsock2.h>

using namespace std;

void hello(SOCKET clientSocket)
{
    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Client: " << buffer << endl;

    string msg = "Hello Client!";
    send(clientSocket, msg.c_str(), msg.length(), 0);
}

void fileTransfer(SOCKET clientSocket)
{
    char fileName[256] = {0};

    recv(clientSocket, fileName, sizeof(fileName), 0);

    string name = "received_" + string(fileName);

    ofstream file(name, ios::binary);

    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
    {
        file.write(buffer, bytesReceived);

        if (bytesReceived < sizeof(buffer))
            break;
    }

    file.close();

    string msg = "File received successfully!";
    send(clientSocket, msg.c_str(), msg.length(), 0);

    cout << "File received: " << name << endl;
}

void arithmetic(SOCKET clientSocket)
{
    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);
    int choice = stoi(buffer);

    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    double num1 = stod(buffer);

    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    double num2 = stod(buffer);

    double result;

    if (choice == 1)
        result = num1 + num2;
    else if (choice == 2)
        result = num1 - num2;
    else if (choice == 3)
        result = num1 * num2;
    else if (choice == 4)
    {
        if (num2 == 0)
        {
            string msg = "Division by zero not possible";
            send(clientSocket, msg.c_str(), msg.length(), 0);
            return;
        }

        result = num1 / num2;
    }
    else
    {
        string msg = "Invalid choice";
        send(clientSocket, msg.c_str(), msg.length(), 0);
        return;
    }

    string resultString = to_string(result);

    send(clientSocket, resultString.c_str(), resultString.length(), 0);
}

void trigonometry(SOCKET clientSocket)
{
    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);
    int choice = stoi(buffer);

    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);

    double angle = stod(buffer);

    double radians = angle * 3.141592653589793 / 180.0;
    double result;

    if (choice == 1)
        result = sin(radians);
    else if (choice == 2)
        result = cos(radians);
    else if (choice == 3)
        result = tan(radians);
    else
    {
        string msg = "Invalid choice";
        send(clientSocket, msg.c_str(), msg.length(), 0);
        return;
    }

    string resultString = to_string(result);

    send(clientSocket, resultString.c_str(), resultString.length(), 0);
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    cout << "Server started..." << endl;
    cout << "Waiting for client..." << endl;

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    cout << "Client connected!" << endl;

    while (true)
    {
        char buffer[1024] = {0};

        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytes <= 0)
            break;

        int choice = stoi(buffer);

        if (choice == 1)
            hello(clientSocket);

        else if (choice == 2)
            fileTransfer(clientSocket);

        else if (choice == 3)
            arithmetic(clientSocket);

        else if (choice == 4)
            trigonometry(clientSocket);

        else if (choice == 5)
            break;

        else
        {
            string msg = "Invalid choice";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}