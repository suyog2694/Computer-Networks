
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6000);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 1);

    cout << "TCP Receiver waiting...\n";

    SOCKET client = accept(server, nullptr, nullptr);
    char buffer[4096];
    int total = 0;

    while (true) {
        int n = recv(client, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        total += n;
    }

    cout << "Received bytes: " << total << '\n';

    closesocket(client);
    closesocket(server);
    WSACleanup();
    return 0;
}
