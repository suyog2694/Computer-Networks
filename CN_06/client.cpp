#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

void hello(SOCKET clientSocket)
{
    string msg = "Hello Server!";

    send(clientSocket, msg.c_str(), msg.length(), 0);

    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Server: " << buffer << endl;
}

void fileTransfer(SOCKET clientSocket)
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

    send(clientSocket, fileName.c_str(), fileName.length(), 0);

    char buffer[1024];

    while (file.read(buffer, sizeof(buffer)))
    {
        send(clientSocket, buffer, file.gcount(), 0);
    }

    if (file.gcount() > 0)
        send(clientSocket, buffer, file.gcount(), 0);

    file.close();

    char response[1024] = {0};

    recv(clientSocket, response, sizeof(response), 0);

    cout << "Server: " << response << endl;
}

void arithmetic(SOCKET clientSocket)
{
    int choice;
    double num1, num2;

    cout << "\n1. Addition" << endl;
    cout << "2. Subtraction" << endl;
    cout << "3. Multiplication" << endl;
    cout << "4. Division" << endl;

    cout << "Enter choice: ";
    cin >> choice;

    cout << "Enter first number: ";
    cin >> num1;

    cout << "Enter second number: ";
    cin >> num2;

    string choiceString = to_string(choice);
    string num1String = to_string(num1);
    string num2String = to_string(num2);

    send(clientSocket, choiceString.c_str(), choiceString.length(), 0);
    send(clientSocket, num1String.c_str(), num1String.length(), 0);
    send(clientSocket, num2String.c_str(), num2String.length(), 0);

    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Result: " << buffer << endl;
}

void trigonometry(SOCKET clientSocket)
{
    int choice;
    double angle;

    cout << "\n1. sin" << endl;
    cout << "2. cos" << endl;
    cout << "3. tan" << endl;

    cout << "Enter choice: ";
    cin >> choice;

    cout << "Enter angle in degrees: ";
    cin >> angle;

    string choiceString = to_string(choice);
    string angleString = to_string(angle);

    send(clientSocket, choiceString.c_str(), choiceString.length(), 0);
    send(clientSocket, angleString.c_str(), angleString.length(), 0);

    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Result: " << buffer << endl;
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    connect(clientSocket,
            (sockaddr*)&serverAddress,
            sizeof(serverAddress));

    cout << "Connected to server!" << endl;

    while (true)
    {
        cout << "\nTCP socket program : " << endl;
        cout << "1. Say Hello" << endl;
        cout << "2. File Transfer" << endl;
        cout << "3. Arithmetic Calculator" << endl;
        cout << "4. Trigonometry Calculator" << endl;
        cout << "5. Exit" << endl;

        int choice;

        cout << "\nEnter choice: ";
        cin >> choice;

        string choiceString = to_string(choice);

        send(clientSocket,
             choiceString.c_str(),
             choiceString.length(),
             0);

        if (choice == 1)
            hello(clientSocket);

        else if (choice == 2)
            fileTransfer(clientSocket);

        else if (choice == 3)
            arithmetic(clientSocket);

        else if (choice == 4)
            trigonometry(clientSocket);

        else if (choice == 5)
        {
            cout << "Connection closed." << endl;
            break;
        }

        else
        {
            char buffer[1024] = {0};

            recv(clientSocket, buffer, sizeof(buffer), 0);

            cout << "Server: " << buffer << endl;
        }
    }

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}