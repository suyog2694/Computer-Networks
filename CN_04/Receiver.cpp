
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5001;
const int TOTAL_PACKETS = 6;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&server, sizeof(server)) ==
        SOCKET_ERROR) {
        cout << "Bind failed.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    bool received[TOTAL_PACKETS] = {false};
    int expected = 0;

    cout << "Selective Repeat Receiver Started\n";

    while (expected < TOTAL_PACKETS) {
        char buffer[1024]{};
        sockaddr_in sender{};
        int senderLen = sizeof(sender);

        int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&sender, &senderLen);

        if (bytes <= 0)
            continue;

        buffer[bytes] = '\0';
        string packet(buffer);

        if (packet.rfind("DATA:", 0) != 0)
            continue;

        int seq = stoi(packet.substr(5));

        if (seq >= 0 && seq < TOTAL_PACKETS) {
            if (!received[seq]) {
                received[seq] = true;
                cout << "Received packet " << seq << '\n';
            } else {
                cout << "Duplicate packet " << seq << '\n';
            }

            string ack = "ACK:" + to_string(seq);

            sendto(sock, ack.c_str(), ack.size(), 0,
                   (sockaddr*)&sender, senderLen);

            while (expected < TOTAL_PACKETS &&
                   received[expected]) {
                expected++;
            }
        }
    }

    cout << "All packets received.\n";

    closesocket(sock);
    WSACleanup();

    return 0;
}
