
#include <iostream>
#include <winsock2.h>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 5001;
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

    vector<bool> acked(TOTAL_PACKETS, false);
    vector<bool> sent(TOTAL_PACKETS, false);

    int base = 0;

    DWORD timeout = 1500;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
               (char*)&timeout, sizeof(timeout));

    cout << "Selective Repeat Sender Started\n";

    while (base < TOTAL_PACKETS) {
        int limit = min(base + WINDOW_SIZE, TOTAL_PACKETS);

        for (int i = base; i < limit; i++) {
            if (!sent[i] && !acked[i]) {
                string packet = "DATA:" + to_string(i);

                sendto(sock, packet.c_str(), packet.size(), 0,
                       (sockaddr*)&receiver, sizeof(receiver));

                cout << "Sent packet " << i << '\n';
                sent[i] = true;
            }
        }

        char buffer[1024]{};
        sockaddr_in from{};
        int fromLen = sizeof(from);

        int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&from, &fromLen);

        if (bytes > 0) {
            buffer[bytes] = '\0';
            string msg(buffer);

            if (msg.rfind("ACK:", 0) == 0) {
                int ackNum = stoi(msg.substr(4));

                if (ackNum >= 0 && ackNum < TOTAL_PACKETS) {
                    acked[ackNum] = true;
                    cout << "Received ACK " << ackNum << '\n';
                }
            }
        } else {
            for (int i = base; i < limit; i++) {
                if (sent[i] && !acked[i]) {
                    string packet = "DATA:" + to_string(i);

                    sendto(sock, packet.c_str(), packet.size(), 0,
                           (sockaddr*)&receiver, sizeof(receiver));

                    cout << "Timeout: Retransmitted packet "
                         << i << '\n';
                }
            }
        }

        while (base < TOTAL_PACKETS && acked[base]) {
            base++;
        }
    }

    cout << "All packets acknowledged.\n";

    closesocket(sock);
    WSACleanup();

    return 0;
}
