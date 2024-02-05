#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    int result;

    // 初始化Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 绑定套接字到IP地址和端口
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080); // 监听端口8080
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // 使用任何可用的地址

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // 监听
    listen(listening, SOMAXCONN);

    // 等待连接
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // 客户端的远程名称
    char service[NI_MAXSERV];   // 服务（即端口）客户端连接的

    ZeroMemory(host, NI_MAXHOST); // 与memset相同
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    // 关闭监听套接字
    closesocket(listening);

    // 接收和发送数据...

    // 清理
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
