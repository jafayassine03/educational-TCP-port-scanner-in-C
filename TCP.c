#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

int is_port_open(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
        return 0;

    struct sockaddr_in target;

    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    target.sin_addr.s_addr = inet_addr(ip);

    int result = connect(sock, (struct sockaddr *)&target, sizeof(target));

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return result == 0;
}

int main() {
    char ip[64];
    int start_port;
    int end_port;

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    printf("=== Simple TCP Port Scanner ===\n\n");

    printf("Enter target IP: ");
    scanf("%63s", ip);

    printf("Enter start port: ");
    scanf("%d", &start_port);

    printf("Enter end port: ");
    scanf("%d", &end_port);

    printf("\nScanning %s...\n\n", ip);

    for (int port = start_port; port <= end_port; port++) {
        if (is_port_open(ip, port)) {
            printf("Port %d is OPEN\n", port);
        }
    }

    printf("\nFinished scanning.\n");

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
