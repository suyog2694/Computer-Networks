
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(6000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << "Connecting to TCP receiver...\n";

    if (connect(sock, (sockaddr*)&server, sizeof(server)) ==
        SOCKET_ERROR) {
        cout << "Connection failed. Start receiver first.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    vector<char> data(10 * 1024 * 1024, 'A');
    int sent = 0;
    int total = static_cast<int>(data.size());

    while (sent < total) {
        int n = send(sock, data.data() + sent, total - sent, 0);
        if (n <= 0) break;
        sent += n;
    }

    cout << "Bytes sent: " << sent << '\n';

    shutdown(sock, SD_SEND);
    closesocket(sock);
    WSACleanup();
    return 0;
}
