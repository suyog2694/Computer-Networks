#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

void fileTransfer(SOCKET serverSocket, sockaddr_in clientAddress)
{
    char fileName[256] = {0};
    int clientLength = sizeof(clientAddress);

    recvfrom(serverSocket, fileName, sizeof(fileName), 0,
             (sockaddr*)&clientAddress, &clientLength);

    cout << "Receiving file: " << fileName << endl;

    string outputFile = "received_" + string(fileName);
    ofstream file(outputFile, ios::binary);

    if (!file)
    {
        cout << "Unable to create file!" << endl;
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while (true)
    {
        bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
                                 (sockaddr*)&clientAddress, &clientLength);

        if (bytesReceived <= 0)
            break;

        if (bytesReceived == 3 &&
            buffer[0] == 'E' &&
            buffer[1] == 'N' &&
            buffer[2] == 'D')
        {
            break;
        }

        file.write(buffer, bytesReceived);
    }

    file.close();

    string message = "File received successfully!";
    sendto(serverSocket, message.c_str(), message.length(), 0,
           (sockaddr*)&clientAddress, clientLength);

    cout << "File received successfully: " << outputFile << endl;
}

int main()
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddress,
             sizeof(serverAddress)) == SOCKET_ERROR)
    {
        cout << "Bind failed!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "UDP Server started..." << endl;
    cout << "Waiting for file..." << endl;

    while (true)
    {
        sockaddr_in clientAddress;
        int clientLength = sizeof(clientAddress);

        char choice[10] = {0};

        int bytes = recvfrom(serverSocket, choice, sizeof(choice), 0,
                             (sockaddr*)&clientAddress, &clientLength);

        if (bytes <= 0)
            break;

        int option = stoi(choice);

        if (option >= 1 && option <= 4)
        {
            fileTransfer(serverSocket, clientAddress);
        }
        else if (option == 5)
        {
            cout << "Server stopped." << endl;
            break;
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}