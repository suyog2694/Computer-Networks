#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

void fileTransfer(SOCKET clientSocket, sockaddr_in serverAddress)
{
    string fileName;

    cout << "Enter file name/path: ";
    cin >> fileName;

    ifstream file(fileName, ios::binary);

    if (!file)
    {
        cout << "File not found!" << endl;
        return;
    }

    sendto(clientSocket, fileName.c_str(), fileName.length(), 0,
           (sockaddr*)&serverAddress, sizeof(serverAddress));

    char buffer[BUFFER_SIZE];

    while (file.read(buffer, sizeof(buffer)))
    {
        sendto(clientSocket, buffer, file.gcount(), 0,
               (sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    if (file.gcount() > 0)
    {
        sendto(clientSocket, buffer, file.gcount(), 0,
               (sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    string endMessage = "END";

    sendto(clientSocket, endMessage.c_str(), endMessage.length(), 0,
           (sockaddr*)&serverAddress, sizeof(serverAddress));

    file.close();

    char response[BUFFER_SIZE] = {0};
    int serverLength = sizeof(serverAddress);

    recvfrom(clientSocket, response, sizeof(response), 0,
             (sockaddr*)&serverAddress, &serverLength);

    cout << "Server: " << response << endl;
}

int main()
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    inet_pton(AF_INET, "192.168.1.37", &serverAddress.sin_addr);

    cout << "UDP Client started..." << endl;

    while (true)
    {
        cout << "\nUDP File Transfer" << endl;
        cout << "1. Script File" << endl;
        cout << "2. Text File" << endl;
        cout << "3. Audio File" << endl;
        cout << "4. Video File" << endl;
        cout << "5. Exit" << endl;

        int choice;

        cout << "\nEnter choice: ";
        cin >> choice;

        string choiceString = to_string(choice);

        sendto(clientSocket, choiceString.c_str(),
               choiceString.length(), 0,
               (sockaddr*)&serverAddress,
               sizeof(serverAddress));

        if (choice >= 1 && choice <= 4)
        {
            fileTransfer(clientSocket, serverAddress);
        }
        else if (choice == 5)
        {
            cout << "Connection closed." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}