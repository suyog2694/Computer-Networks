#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    const char* HOST = "127.0.0.1";
    const int PORT = 5005;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock initialization failed.\n";
        return 1;
    }

    // Create a UDP socket
    SOCKET receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (receiver == INVALID_SOCKET) {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Configure receiver address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &serverAddr.sin_addr);

    // Bind socket to IP and port
    if (bind(receiver, (sockaddr*)&serverAddr,
             sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed. Error: " << WSAGetLastError() << '\n';
        closesocket(receiver);
        WSACleanup();
        return 1;
    }

    cout << "UDP receiver listening on "
         << HOST << ":" << PORT << '\n';

    char buffer[4096];
    sockaddr_in senderAddr{};
    int senderLen = sizeof(senderAddr);

    while (true) {
        int bytesReceived = recvfrom(
            receiver, buffer, sizeof(buffer), 0,
            (sockaddr*)&senderAddr, &senderLen
        );

        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Receive failed.\n";
            break;
        }

        char senderIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &senderAddr.sin_addr,
                  senderIP, sizeof(senderIP));

        cout << "\n--- Packet Received ---\n";
        cout << "Sender IP: " << senderIP << '\n';
        cout << "Sender Port: " << ntohs(senderAddr.sin_port) << '\n';

        string message(buffer, bytesReceived);
        cout << "Message: " << message << '\n';
        cout << "Payload Size: " << bytesReceived << " bytes\n";
    }

    closesocket(receiver);
    WSACleanup();

    return 0;
}
