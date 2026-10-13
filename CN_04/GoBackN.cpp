
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5000;
const int TOTAL_PACKETS = 6;
const int WINDOW_SIZE = 3;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in receiver{};
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    int base = 0;
    int nextSeq = 0;

    cout << "Go-Back-N Sender Started\n";

    while (base < TOTAL_PACKETS) {
        while (nextSeq < base + WINDOW_SIZE &&
               nextSeq < TOTAL_PACKETS) {
            string packet = "DATA:" + to_string(nextSeq);

            sendto(sock, packet.c_str(), packet.size(), 0,
                   (sockaddr*)&receiver, sizeof(receiver));

            cout << "Sent packet " << nextSeq << '\n';
            nextSeq++;
        }

        cout << "Waiting for ACK...\n";

        char buffer[1024]{};
        sockaddr_in from{};
        int fromLen = sizeof(from);

        // Wait up to 2 seconds for an ACK.
        DWORD timeout = 2000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
                   (char*)&timeout, sizeof(timeout));

        int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&from, &fromLen);

        if (bytes <= 0) {
            cout << "Timeout! Retransmitting from packet "
                 << base << '\n';
            nextSeq = base;
            continue;
        }

        buffer[bytes] = '\0';
        string ack(buffer);

        if (ack.rfind("ACK:", 0) == 0) {
            int ackNum = stoi(ack.substr(4));

            if (ackNum >= base && ackNum < nextSeq) {
                base = ackNum + 1;
                cout << "Received ACK " << ackNum << '\n';
            }
        }
    }

    cout << "All packets acknowledged.\n";

    closesocket(sock);
    WSACleanup();

    return 0;
}
