#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    int result;

    // ��ʼ��Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // �����׽���
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // ���׽��ֵ�IP��ַ�Ͷ˿�
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080); // �����˿�8080
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // ʹ���κο��õĵ�ַ

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // ����
    listen(listening, SOMAXCONN);

    // �ȴ�����
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // �ͻ��˵�Զ������
    char service[NI_MAXSERV];   // ���񣨼��˿ڣ��ͻ������ӵ�

    ZeroMemory(host, NI_MAXHOST); // ��memset��ͬ
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    // �رռ����׽���
    closesocket(listening);

    // ���պͷ�������...

    // ����
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
