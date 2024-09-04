#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
 
#pragma comment(lib, "ws2_32.lib")
 
#define PORT 8888
 
int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[2] = { 0 };
 
    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }
 
    // 创建 socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }
 
    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
 
    // 绑定 socket
    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Socket bind failed.\n";
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
 
    std::cout << "Server is listening...\n";
 
    // 接收来自客户端的数据并显示在屏幕上
    while (true) {
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen) != SOCKET_ERROR) {
            if (isprint(buffer[0])) { // 可见字符
                if (buffer[1] == 0) {
                    std::cout << "Key pressed: " << buffer[0] << " (Press)" << "\n";
                } else {
                    std::cout << "Key pressed: " << buffer[0] << " (Release)" << "\n";
                }
            } else {
                std::cout << "Key pressed: ASCII " << static_cast<int>(buffer[0]);
                if (buffer[1] == 0) {
                    std::cout << " (Press)" << "\n";
                } else {
                    std::cout << " (Release)" << "\n";
                }
            }
        }
    }
 
    closesocket(sockfd);
    WSACleanup();
 
    return 0;
}
 
